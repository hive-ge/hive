<h1 align=center> HIVE </h1>

## Sandbox Engine

Hive is a general purpose sandbox for creating digital content with a focus on 2D based images.

Written in C++

Capabilities:

- Node and Graph Tools
- Audio
- Text
- Input
- File Saving
- Custom Scripting, possibly based on Lua
- OpenGL graphics system.
- Note System?


## Nodes
Any object the can interact with another object through simple message passing. 
    Physics, AI, and other scrutable events can benefit from a node based interpretation? 
    What do you mean scrutable? 
    How big should a message be? How can this be generalized?

## Drones

I believe I want a message system that doubles as a base system
for concurrent and parallel programming tasks within CPU and 
GPU space. To allow for this, I'll need to create a spec for a 
universal computational unit that provides enough flexibility to 
handle different forms of processing, including passing message
indicators that point to external resources. I think a 128 bit
unit is sufficient to provide enough bit space to be useful without
being two restrictive in what can be done with it. In a 64 bit architecture
128 bit's could be a double or single fetch object. 

Maybe call it a drone unit?

# SmartProgram

## usage

```c++

using namespace hive::gpu::gl

std::string vector = "Vector Shader";
std::string fragment = "Fragment Shader";

SmartGLProgram program = SmartcreateShaderProgram(vector, fragment);



```


Use `HIVE_DEBUG` for debug statements.

##### example
```c++
/* ...Release code... */

//############################## DEBUG
#ifdef HIVE_DEBUG

/* ...Debug code... */

#endif 
//############################## END DEBUG

/* ...Release code... */
```