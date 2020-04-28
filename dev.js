console.log(global);

import electron from "electron";
import proc from "child_process";

//const electron = require('electron');

//const proc = require('child_process');

const child = proc.spawn(electron, [...process.argv.slice(2), "./source/frontend/hive_frontend_entry.js"], { stdio: 'inherit', windowsHide: false });

child.on('close', function (code) {
    process.exit(code);
});

const handleTerminationSignal = function (signal) {
    process.on(signal, function signalHandler() {
        if (!child.killed) {
            child.kill(signal);
        }
    });
};

handleTerminationSignal('SIGINT');
handleTerminationSignal('SIGTERM');
