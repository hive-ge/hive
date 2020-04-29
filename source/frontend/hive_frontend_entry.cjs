/**
 * Hive Frontend Entry Point
 *
 * (c) Anthony C Weathersby 2020
 */

//Run HIVE editor environment as a child process. 

//Load up the editing environment 

//import Q from "@nodegui/nodegui";

const hive = require("../../build/Debug/hive_dev.node");
const fs = require("fs");
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

    console.log(hive);

    const drone = new hive.Drone();

    const prop = new hive.Prop(223);
    const l = new hive.Prop;
    prop.next = l;

    console.log(prop.byte_size);
    console.log(l.byte_size);
    console.log(prop.next, prop.next.constructor == prop.constructor);
    console.log(prop.type, prop.next.byte_size);

    console.log(prop instanceof hive.Prop);
    console.log(prop.next.next);

    try {
        console.log(prop.gravy(prop));
    } catch (e) {
        console.log(e);
    }

    hive.addShouldCloseCB(() => {
        console.log("Received signal to close app.");
        //win.close();
        process.exit(0);
    });

})();

