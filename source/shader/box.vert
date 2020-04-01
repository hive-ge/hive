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
}