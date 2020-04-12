/**
 * Hive Frontend Entry Point
 *
 * (c) Anthony C Weathersby 2020
 */

//Run HIVE editor environment as a child process. 

//Load up the editing environment 

const
    electron = require("electron"),
    { app, BrowserWindow } = electron,
    hive = require("../../build/Release/hive_dev"),
    fs = require("fs");

fsp = fs.promises;

(async function () {

    hive.init();

    await app.whenReady();

    const win = new BrowserWindow({
        paintWhenInitiallyHidden: true,
        // hidden: true,
        frame: false,
        webPreferences: { offscreen: true, devTools: true, nodeIntegration: true }
    });

    win.setIgnoreMouseEvents(true);

    win.webContents.on('paint', (event, dirty, image) => {
        //load bitmap image to C++ buffer;



        console.log();

        fsp.writeFile("./test.jpg", image.toJPEG(100), {});

        // updateBitmap(dirty, image.getBitmap())
    });

    win.webContents.frameRate = 60;

    win.loadURL(`file:///${__dirname}/ui/ui_entry.html`);

    hive.addShouldCloseCB(() => {
        console.log("Received signal to close app.");
        win.close();
        process.exit(0);
    });

})();

