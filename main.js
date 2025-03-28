const { app, dialog} = require('electron');
const { exec } = require('child_process');
const path = require('path');
const AutoLaunch = require('auto-launch');

const autoLauncher = new AutoLaunch({
  name: 'KeyboardLights',
  path: process.execPath,
});

autoLauncher.isEnabled().then((isEnabled) => {
  if (!isEnabled) {
    autoLauncher.enable();
  }
}).catch((err) => {
  console.error('auto-launch error:', err);
});

app.whenReady().then(() => {
  const binaryPath = path.join(process.resourcesPath, './scrolllock_on');

  exec(binaryPath, (error, stdout, stderr) => {
    if (error) {
      console.error(`Error: ${error.message}`);
      dialog.showErrorBox('Error while running command:', error.message);
      return;
    }
    if (stderr) {
      console.error(`stderr: ${stderr}`);
    }
    if (stdout) {
      console.log(`stdout: ${stdout}`);
    }

    app.quit();
  });
});

app.on('window-all-closed', () => {});