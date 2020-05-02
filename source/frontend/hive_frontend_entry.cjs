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
    const program = new hive.ShaderProgramProp();
    const prop = new hive.Prop(223);
    const sprite = new hive.SpriteProp(223);
    const l = new hive.Prop;

    prop.next = l;

    program.addVertexShader(`
    
#version 430

layout (std140) uniform  ObjData
{
    vec3 pos;
    vec3 rot;
    vec3 scl;
};
    
in vec2 pixel_position;      

in vec3 vertex;

uniform mat4 camera;

void main() { 
    //gl_Position = camera * vec4((vertex*scl)+vec3(0.0,0.0,0.0), 1.0);// + vec4(0.0,0.0,-500.0,1.0); 
    gl_Position = camera * vec4((vertex*scl)+vec3(pixel_position,0.0), 1.0);//+ vec4(0.0,0.0,-500.0,1.0 ; 
}`);
    program.addFragShader(`

#version 430

out vec4 FragColor; void main(){ 
    FragColor =  vec4(0.5,0.1,0.5,1.0);
}`);

    program.compileShader();

    console.log(program);

    program.load;

    //const sprite = drone.getProp("sprite");

    if (sprite) {

    }


    drone.connect(sprite);
    console.log("sprite type", sprite.type);
    console.log(prop.byte_size);
    console.log(l.byte_size);
    console.log(prop.next, prop.next.constructor == prop.constructor);
    console.log(prop.type, prop.next.byte_size);
    console.log(prop instanceof hive.Prop);
    console.log(prop.next.next);
    console.log(sprite instanceof hive.Prop);

    try {
        // console.log(prop.gravy(prop));
    } catch (e) {
        console.log(e);
    }

    hive.addShouldCloseCB(() => {
        console.log("Received signal to close app.");
        //win.close();
        process.exit(0);
    });

})();

