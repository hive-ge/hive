#include "includes.hpp"

using namespace hive;

std::string shader_string(
    "[vert]\n"
    "#version 300 es\n"
    ""
    "in vec3 vertex;"
    ""
    "uniform float rot;"
    ""
    "out vec2 tex_coord;"
    ""
    "void main()"
    "{"
    ""
    "gl_Position = vec4(vertex.xy * vec2(cos(rot), cos(rot)) + vertex.yx * vec2(-sin(rot), "
    "sin(rot)),"
    "(vertex.z), 1.0);"
    ""
    "tex_coord = vec2(1., 1.) - (vertex.xy + vec2(1.0, 1.0)) / 2.0;"
    "}"
    ""
    "[frag]\n"
    "#version 300 es\n"
    "precision mediump float;"
    ""
    "uniform sampler2D color_t;"
    ""
    "in vec2 tex_coord;"
    ""
    "out vec4 FragColor;"
    ""
    "void main() { FragColor = vec4(0.1, 0.8, 0.1, 1.0) + texture(color_t, tex_coord); }");


// Create Global Memory Object.
hive::DataPool hive::general_data_pool(4096);

int main(int arg_ct, char ** args)
{
    hive::DroneDataPool pool(120); // Allocate enough space for 120 data elements of 32 bytes.

    // Setup bosses

    BigBadBoss boss;

    hive::interface::InterfaceBoss interface_boss(1280, 720);

    RenderBoss render;

    boss.setup();

    // Setup first game object

    Drone::Ref drone = pool.createObjectReturnRef<Drone>();

    ShaderProgramProp::Ref shader = DroneDataPool::createObjectReturnRef<ShaderProgramProp>();
    drone->connect(shader);

    shader->fromString(shader_string);

    RenderableProp::Ref renderable = pool.createObjectReturnRef<RenderableProp>();
    drone->connect(renderable);
    renderable->SET_RENDER_STATE(true);

    MeshProp::Ref mesh_prop = pool.createObjectReturnRef<MeshProp>();
    mesh_prop->addVertex(1, .5, 0, 1.0, 1.0);
    mesh_prop->addVertex(-1, 1.0, 0, 1.0, 1.0);
    mesh_prop->addVertex(-1, -1.0, 0, 1.0, 1.0);
    mesh_prop->addVertex(1, -1.0, 0, 1.0, 1.0);
    mesh_prop->addTriangle(0, 1, 2);
    mesh_prop->addTriangle(2, 3, 0);
    drone->connect(mesh_prop);


    FloatProp::Ref float_prop = pool.createObjectReturnRef<FloatProp>();
    float_prop->setTag("rot");
    drone->connect(float_prop);


    // Setup the render layer
    Drone::Ref drone_layer                      = pool.createObjectReturnRef<Drone>();
    RenderableProp::Ref renderable_layer        = pool.createObjectReturnRef<RenderableProp>();
    RenderLayerProp::Ref renderable_layer_layer = pool.createObjectReturnRef<RenderLayerProp>();
    ImageProp::Ref image                        = pool.createObjectReturnRef<ImageProp>();

    drone_layer->connect(renderable_layer);
    drone_layer->connect(image);
    drone_layer->connect(renderable_layer_layer);

    image->data->width  = 1280;
    image->data->height = 720;

    renderable_layer->SET_RENDER_STATE(true);


    // AudioBoss
    // AnimationBoss
    // AssetBoss

    int timout = 1200;

    float i = 0.0;

    while (boss.update() && timout-- > 0) {
        // float_prop->set(i += 0.01);
    }


    SUCCESS();
}