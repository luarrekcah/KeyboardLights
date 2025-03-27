const { app } = require('electron');
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
  const command = path.join(__dirname, 'setleds -caps +scroll ^num');

  exec(command, (error, stdout, stderr) => {
    if (error) {
      console.error(`Error: ${error.message}`);
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