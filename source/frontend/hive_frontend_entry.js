/**
 * Hive Frontend Entry Point
 *
 * (c) Anthony C Weathersby 2020
 */

//Run HIVE editor environment as a child process. 

//Load up the editing environment 

//import Q from "@nodegui/nodegui";

import hive from "../../build/Release/hive_dev";
import fs from "fs";
//import hive from ;
//import fs from "fs";

//const { QLabel, QMainWindow } = Q;


fsp = fs.promises;

(async function () {

    hive.init();

    //const win = new QMainWindow();

    // const label = new QLabel(win);

    // win.show();

    //global.win = win;

    console.log(Prop);
    
    hive.addShouldCloseCB(() => {
        console.log("Received signal to close app.");
        //win.close();
        process.exit(0);
    });

})();