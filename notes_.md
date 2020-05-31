<h1 align=center>
    <img src="./design/logo/hive_logo_alpha.svg" type="text/svg" rel="svg" height=481>
</h1>

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


## Errors are Key

Everything that can throw an errors should, or at least report that an error has accurred. Without knowing if something did not perform as expected, hidden problems can easily crop that can steal valueble development. By reporting anything that is not correct, the dev gets invaluable information on what to fix when something breaks.

## Compute shaders

Must be defined with `#version 430` or higher.

Thread groups are 3D blocks of threads. Defined in shader. Own definition

Each thread group gets input information to allow it to spatially locate it amongst the data sets.

Work groups are 3D blocks of Thread Groups. 

Work Invocation / Work Item - single thread.

In C/C++ space, use `glMemoryBarrier` to sync CPU/GPU execution before reading glBuffer data.
- `in uvec3 gl_NumWorkGroups;`
- `in uvec3 gl_GlobalInvocationID` the location of the thread across all work groups. values `x` `y` `z` and combinations thereof.
- `in uvec3 gl_WorkGroupID ;`the location of the local work group across all work work groups. [link](https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/gl_WorkGroupID.xhtml)
- `in uvec3 gl_LocalInvocationID;` the location of the work invocation within the local work group. [link](https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/gl_LocalInvocationID.xhtml)
- `in uint  gl_LocalInvocationIndex;` 1 dimensional index of the invocation unit within the total number of invocations:
```c
gl_LocalInvocationIndex =
        gl_LocalInvocationID.z * gl_WorkGroupSize.x * gl_WorkGroupSize.y +
        gl_LocalInvocationID.y * gl_WorkGroupSize.x + 
        gl_LocalInvocationID.x;
```
## Terms 
 
### Local Work Group

Defined by `layout(local_size_x, local_size_y, local_size_z)`.

```c
layout(local_size_x = 1, local_size_y = 1) in;
```

### Shared Memory

Declared with `shared` qualifier. 
Cannot have initializer: `shared float d = 0.1; //not gonna work`. Has to be set by one thread.
Need to synch execution for shared memory, not just use barriers. User `barrier()` to ensure execution across invocations syncs before continuing.

#### Memory Barriers
- `memoryBarrier()`
- `memoryBarrierShared()` - Orders read write for current workgroup.


```c
shared vec4 data
```

atomicAdd doesn't appear to handle multiple reads writes. Try a dense grid write first approach