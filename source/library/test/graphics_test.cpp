#include "includes.hpp"

using namespace hive;

std::string shader_string("[vert]\n"
                          "#version 300 es\n"
                          ""
                          "in vec3 vertex;"
                          ""
                          "uniform float rot;"
                          ""
                          "out vec2 tex_coord;"
                          "uniform mat4 camera;"
                          ""
                          "void main()"
                          "{"
                          ""
                          "gl_Position = camera * vec4(vertex, 1.0);"
                          ""
                          "tex_coord = (vertex.xy + vec2(1.0, 1.0));"
                          "}"
                          ""
                          "[frag]\n"
                          "#version 300 es\n"
                          "precision mediump float;"
                          ""
                          ""
                          "in vec2 tex_coord;"
                          ""
                          "out vec4 FragColor;"
                          ""
                          "void main() { FragColor = vec4(1.0, 0.25, 0.75, 1.0); }");

std::string shader_string2(
    "[vert]\n"
    "#version 300 es\n"
    ""
    "in vec3 vertex;"
    "in vec2 UV;"
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
    "tex_coord = UV;"
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
    "void main() { FragColor =  texture(color_t, tex_coord) * vec4(1.0, 1.0, 1.0, 1.0) + vec4(0.5, "
    "0.0, 0.0, 1.0) ; }");

#define PI 3.14159265359


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

    // Setup the camera
    Drone::Ref camera                     = pool.createObjectReturnRef<Drone>();
    Mat44FloatProp::Ref matrix            = pool.createObjectReturnRef<Mat44FloatProp>();
    Mat44FloatProp::Ref matrix2           = pool.createObjectReturnRef<Mat44FloatProp>();
    RenderableProp::Ref camera_renderable = pool.createObjectReturnRef<RenderableProp>();

    matrix->setTag("camera"); // projection matrix

    matrix->toPerspectiveProjectionMatrix(1280, 720, PI / 2.0 /* 90deg */, 0.01, 100.0);

    auto matA   = matrix->get();
    mat44f matB = {};
    matB.row4.z = -10.0;
    matrix->set(matA * matB);
    matrix->set(matA * matB);

    matrix2->setTag("lens");
    camera->connect(matrix);
    camera->connect(matrix2);
    camera->connect(camera_renderable);

    camera_renderable->setRenderPassGroup(1);
    camera_renderable->SET_RENDER_STATE(true);


    // Setup first game object

    Drone::Ref drone = pool.createObjectReturnRef<Drone>();

    ShaderProgramProp::Ref shader = DroneDataPool::createObjectReturnRef<ShaderProgramProp>();
    drone->connect(shader);

    shader->fromString(shader_string);

    RenderableProp::Ref renderable = pool.createObjectReturnRef<RenderableProp>();
    drone->connect(renderable);
    renderable->setRenderPassGroup(1);
    renderable->SET_RENDER_STATE(true);
    MeshProp::Ref mesh_prop = pool.createObjectReturnRef<MeshProp>();
    mesh_prop->addVertex(1.0, 0.5, 0, 1.0, 1.0);
    mesh_prop->addVertex(-0.5, 0.5, 0, 0.5, 0.5);
    mesh_prop->addVertex(-1.0, -0.5, 0, 0.5, 0.5);
    mesh_prop->addVertex(0.5, -0.5, 0, 0.5, 0.5);
    mesh_prop->addTriangle(0, 1, 2);
    mesh_prop->addTriangle(2, 3, 0);
    drone->connect(mesh_prop);
    FloatProp::Ref float_prop = pool.createObjectReturnRef<FloatProp>();
    float_prop->setTag("rot");
    drone->connect(float_prop);


    // Setup the first render layer
    Drone::Ref drone_layer                      = pool.createObjectReturnRef<Drone>();
    RenderableProp::Ref renderable_layer        = pool.createObjectReturnRef<RenderableProp>();
    RenderLayerProp::Ref renderable_layer_layer = pool.createObjectReturnRef<RenderLayerProp>();
    ImageProp::Ref image                        = pool.createObjectReturnRef<ImageProp>();
    drone_layer->connect(renderable_layer);
    drone_layer->connect(image);
    drone_layer->connect(renderable_layer_layer);
    image->data->width  = 1280;
    image->data->height = 720;
    renderable_layer->setRenderPassGroup(1);
    renderable_layer->SET_RENDER_STATE(true);


    // Setup the second object which will bind to the root layer and render buffer image.

    Drone::Ref drone2 = pool.createObjectReturnRef<Drone>();

    ShaderProgramProp::Ref shader2 = DroneDataPool::createObjectReturnRef<ShaderProgramProp>();
    drone2->connect(shader2);
    shader2->fromString(shader_string2);

    ImageProp::Ref image2 = pool.createObjectReturnRef<ImageProp>();
    image2->instance(image);
    image2->setTag("color_t");

    RenderableProp::Ref renderable2 = pool.createObjectReturnRef<RenderableProp>();
    drone2->connect(renderable2);
    renderable2->SET_RENDER_STATE(true);

    MeshProp::Ref mesh_prop2 = pool.createObjectReturnRef<MeshProp>();
    mesh_prop2->addVertex(0.5625 * .5, .5, 0, 1.0, 1.0);
    mesh_prop2->addVertex(-0.5625 * .5, .5, 0, 0.0, 1.0);
    mesh_prop2->addVertex(-0.5625 * .5, -.5, 0, 0.0, 0.0);
    mesh_prop2->addVertex(0.5625 * .5, -.5, 0, 1.0, 0.0);
    mesh_prop2->addTriangle(0, 1, 2);
    mesh_prop2->addTriangle(2, 3, 0);
    drone2->connect(mesh_prop2);
    drone2->connect(image2);

    renderable2->setRenderPassGroup(0);
    renderable2->SET_RENDER_STATE(true);

    // AudioBoss
    // AnimationBoss
    // AssetBoss

    int timout = 120000000;

    float i = 0.0;


    while (boss.update() && timout-- > 0) {
        // float_prop->set(i += 0.01);
        // matB = {};
        // matB.row4.z = -22.0 + std::cos(i += 0.0002) * -20;
        matrix->toPerspectiveProjectionMatrix(
            1280, 720, PI / (2.2 + 1.0 * std::cos(i += 0.0002)) /* 90deg */, 0.01, 100.0);
        matA = matrix->get();
        matrix->set(matA * matB);
    }


    SUCCESS();
}