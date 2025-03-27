# KeyboardLights for macOS

![KeyboardLights Icon](assets/icon.png)

A simple app to **activate keyboard lights** on macOS.

### Features:
- Enable keyboard lights on boot
- Execute a command to toggle keyboard LEDs

Thanks to [damieng](https://github.com/damieng/setledsmac) for the amazing work on **setledsmac**!

## How to Use

1. **Download and install the app**.
2. The app will automatically set the keyboard LEDs on each boot.
3. No UI is shown as it runs in the background silently.
4. Close the app after keyboard get lights on.

---

## How it Works

This app runs a command to enable keyboard lights at boot by executing:

```bash
setleds -caps +scroll ^num
```