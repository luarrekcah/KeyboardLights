#include <stdio.h>
#include <stdlib.h>
#include <IOKit/hid/IOHIDLib.h>

typedef enum {
    Off = 0,
    On = 1
} LedState;

#define kHIDPage_GenericDesktop     0x01
#define kHIDUsage_GD_Keyboard       0x06
#define kHIDPage_LEDs               0x08
#define kHIDUsage_LED_ScrollLock    0x03

CFMutableDictionaryRef getKeyboardDictionary(void) {
    CFMutableDictionaryRef result = CFDictionaryCreateMutable(kCFAllocatorDefault, 0,
        &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);

    if (!result) return NULL;

    UInt32 page = kHIDPage_GenericDesktop;
    UInt32 usage = kHIDUsage_GD_Keyboard;

    CFNumberRef pageRef = CFNumberCreate(kCFAllocatorDefault, kCFNumberIntType, &page);
    CFNumberRef usageRef = CFNumberCreate(kCFAllocatorDefault, kCFNumberIntType, &usage);

    if (pageRef && usageRef) {
        CFDictionarySetValue(result, CFSTR(kIOHIDDeviceUsagePageKey), pageRef);
        CFDictionarySetValue(result, CFSTR(kIOHIDDeviceUsageKey), usageRef);
    }

    if (pageRef) CFRelease(pageRef);
    if (usageRef) CFRelease(usageRef);

    return result;
}

void enableScrollLock(IOHIDDeviceRef device, CFDictionaryRef keyboardDict) {
    CFArrayRef elements = IOHIDDeviceCopyMatchingElements(device, keyboardDict, kIOHIDOptionsTypeNone);
    if (!elements) return;

    CFIndex count = CFArrayGetCount(elements);
    for (CFIndex i = 0; i < count; i++) {
        IOHIDElementRef element = (IOHIDElementRef)CFArrayGetValueAtIndex(elements, i);
        if (IOHIDElementGetUsagePage(element) == kHIDPage_LEDs &&
            IOHIDElementGetUsage(element) == kHIDUsage_LED_ScrollLock) {

            IOHIDValueRef value = IOHIDValueCreateWithIntegerValue(kCFAllocatorDefault, element, 0, On);
            if (value) {
                IOReturn result = IOHIDDeviceSetValue(device, element, value);
                if (result == kIOReturnSuccess) {
                    printf("Scroll Lock success.\n");
                } else {
                    fprintf(stderr, "Error on toggle LEDs.\n");
                }
                CFRelease(value);
            }
            break;
        }
    }

    CFRelease(elements);
}

int main() {
    IOHIDManagerRef manager = IOHIDManagerCreate(kCFAllocatorDefault, kIOHIDOptionsTypeNone);
    if (!manager) {
        fprintf(stderr, "Error to create IOHIDManager.\n");
        return 1;
    }

    CFDictionaryRef keyboardDict = getKeyboardDictionary();
    IOHIDManagerSetDeviceMatching(manager, keyboardDict);
    IOHIDManagerOpen(manager, kIOHIDOptionsTypeNone);

    CFSetRef devices = IOHIDManagerCopyDevices(manager);
    if (!devices) {
        fprintf(stderr, "No keyboards found.\n");
        return 1;
    }

    CFIndex numDevices = CFSetGetCount(devices);
    IOHIDDeviceRef *deviceRefs = malloc(sizeof(IOHIDDeviceRef) * numDevices);
    CFSetGetValues(devices, (const void **)deviceRefs);

    for (CFIndex i = 0; i < numDevices; i++) {
        enableScrollLock(deviceRefs[i], keyboardDict);
    }

    free(deviceRefs);
    CFRelease(devices);
    CFRelease(keyboardDict);
    CFRelease(manager);

    return 0;
}
