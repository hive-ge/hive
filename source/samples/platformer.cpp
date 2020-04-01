#include "gpu/opengl/buffer.h"
#include <GL/gl.h>
#include <chrono>
#include <hive.h>
#include <iostream>
#include <stdio.h>
#include <vector>

using namespace hive::all;
using std::vector;

#define PIXEL_COUNT 8192

struct Pixel {
    vec2 Position = {500.0, 0.0}; // 0
    vec2 OldPosition;
    vec2 Velocity = {0, 0}; // 2
    vec2 newVel;
    float Buffer;                      // 3
    float Radius              = 1.0;   // 4
    float Mass                = 0.0;   // 5
    float FrictionCoefficient = 0.002; // 6
    int links[4]              = {0, 0, 0, 0};
};

struct Locality {
    int set = 0;
};

void uploadData(SmartGLBuffer & pixel_buff, SmartGLBuffer & locality_buff)
{
    Pixel * pixels        = new Pixel[PIXEL_COUNT];
    Locality * localities = new Locality[4096 * 4096];

    for (auto i = 1; i < PIXEL_COUNT; i++) {
        makeRandom<float>((pixels[i].Position), -40, 80);
        makeRandom<float>((pixels[i].Velocity), -0.10, 0.10);
    }

    // Send data off to the GPU.
    pixel_buff.setData(pixels, sizeof(Pixel) * PIXEL_COUNT);
    locality_buff.setData(localities, sizeof(Locality) * 4096 * 4096);

    delete[] pixels;
    delete[] localities;
}

void linkBuffers(SmartGLProgram & program, SmartGLBuffer & pixel_buff,
                 SmartGLBuffer & locality_buff)
{
    auto pixel_block    = program.getShaderBufferBlock("Pixels");
    auto locality_block = program.getShaderBufferBlock("Localities");

    pixel_block.bind(pixel_buff);
    locality_block.bind(locality_buff);
}

int main(int arg_count, char ** arguments)
{

    hive::interface::GLFWBoss boss(1920, 1080);

    if (!boss.init()) return -1;

    auto controller = boss.getController(0);

    mat44 camera = mat44::perspectiveProjectionMatrix(90, 1920.0 / 1080.0, 0.001, 1000);

    mat44 pos, d;

    pos.m15 = -50.0;

    ProgramDefinition program_definition;
    SmartGLBuffer bufferA, bufferB, bufferC, bufferD;
    //*
    uploadData(bufferA, bufferB);


    std::string str = hive::resource::loadUTF8File(
        "/home/anthony/work/active/apps/hive/source/shader/compute/locate.comp");
    program_definition.comp = &str;
    auto locateProgram      = hive::gl::createShaderProgram(program_definition);
    locateProgram.use();
    linkBuffers(locateProgram, bufferA, bufferB);
    locateProgram.release();

    str = hive::resource::loadUTF8File(
        "/home/anthony/work/active/apps/hive/source/shader/compute/integrate.comp");
    program_definition.comp = &str;
    auto integrateProgram   = hive::gl::createShaderProgram(program_definition);
    integrateProgram.use();
    linkBuffers(integrateProgram, bufferA, bufferB);
    integrateProgram.release();


    str = hive::resource::loadUTF8File(
        "/home/anthony/work/active/apps/hive/source/shader/compute/apply.comp");
    program_definition.comp = &str;
    auto applyProgram       = hive::gl::createShaderProgram(program_definition);
    applyProgram.use();
    linkBuffers(applyProgram, bufferA, bufferB);
    applyProgram.release();


    str = hive::resource::loadUTF8File(
        "/home/anthony/work/active/apps/hive/source/shader/compute/clear.comp");
    program_definition.comp = &str;
    auto clearProgram       = hive::gl::createShaderProgram(program_definition);
    clearProgram.use();
    linkBuffers(clearProgram, bufferA, bufferB);
    clearProgram.release();


    std::string str_vert =
        hive::resource::loadUTF8File("/home/anthony/work/active/apps/hive/source/shader/box.vert");
    std::string str_frag =
        hive::resource::loadUTF8File("/home/anthony/work/active/apps/hive/source/shader/box.frag");
    ProgramDefinition program_definition2;
    program_definition2.vert = &str_vert;
    program_definition2.frag = &str_frag;

    auto renderA = hive::gl::createShaderProgram(program_definition2);

    struct {
        unsigned int indices[6] = {0, 1, 3, 3, 2, 1}; // 24
        vec3 surface_points[4]  = {{-1.0, 1.0, 0.0},
                                  {1.0, 1.0, 0.0},
                                  {1.0, -1.0, 0.0},
                                  {-1.0, -1.0, 0.0}}; // 48
    } dataA;

    struct {
        vec4 v4_pos = {1.0, 1.0, 1.0, 1.0};
        vec4 v4_rot = {1.0, 1.0, 1.0, 1.0};
        vec4 v4_scl = {0.2, 0.2, 0.2, 0.5};
    } dataB;

    renderA.use();

    auto vertexInput = renderA.getInput("vertex");
    auto pixelInput  = renderA.getInput("pixel_position");
    auto obj_data    = renderA.getUniformBlock("ObjData");

    bufferC.use(SKGLB::ELEMENT_ARRAY_BUFFER);

    bufferC.setData(&dataA, 24 + 48);
    bufferD.setData(&dataB, 48);

    vertexInput.use(bufferC, GL_FLOAT, 0, 24);

    pixelInput.use(bufferA, GL_FLOAT, sizeof(Pixel), offsetof(Pixel, Position), 1);

    auto camera_uni = renderA.getUniform("camera");

    camera_uni.use();

    obj_data.bind(bufferD, 0, 0, 48);

    // making background color black at first

    glClearColor(0.0, 0.0, 0.0, 1.0);

    while (boss.canRun()) {

        boss.update();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        controller.poll();

        // std::cout << controller << std::endl;

        if (controller.hasButtonStateChanged(controller::ButtonName::ONE)) {
            if (!controller.isButtonPressed(controller::ButtonName::ONE)) {
                // Consume button pressed information. This will reset the button state.
                controller.getButtonPressDuration(controller::ButtonName::ONE);
                // Reset position of camera.
                pos.m13 = 0.0;
                pos.m14 = 0.0;
            }
        }

        pos.m13 += (controller.getAxis(controller::AxisName::FOUR) *
                    controller.getAxis(controller::AxisName::FOUR)) *
                   -4.5 * controller.getAxis(controller::AxisName::FOUR);

        pos.m14 += (controller.getAxis(controller::AxisName::FIVE) *
                    controller.getAxis(controller::AxisName::FIVE)) *
                   4.5 * controller.getAxis(controller::AxisName::FIVE);

        pos.m15 = (((controller.getAxis(controller::AxisName::SIX) *
                     controller.getAxis(controller::AxisName::SIX) *
                     controller.getAxis(controller::AxisName::SIX) * 80.)) +
                   -((controller.getAxis(controller::AxisName::THREE) *
                      controller.getAxis(controller::AxisName::THREE) *
                      controller.getAxis(controller::AxisName::THREE) * 80.))) -
                  80.0;


        int i = 0;
        while (i++ < 10) {

            clearProgram.use();
            glDispatchCompute(128, 128, 1);
            if (clearErrors()) throw(-1);
            // glMemoryBarrier(GL_ALL_BARRIER_BITS);

            locateProgram.use();
            glDispatchCompute(PIXEL_COUNT / 32, 1, 1);
            if (clearErrors()) throw(-1);
            // glMemoryBarrier(GL_ALL_BARRIER_BITS);

            //
            integrateProgram.use();
            glDispatchCompute(PIXEL_COUNT / 32, 1, 1);
            if (clearErrors()) throw(-1);
            glMemoryBarrier(GL_ALL_BARRIER_BITS);


            applyProgram.use();
            glDispatchCompute(PIXEL_COUNT / 32, 1, 1);
            if (clearErrors()) throw(-1);
        }

        // glMemoryBarrier(GL_ALL_BARRIER_BITS);
        renderA.use();

        camera_uni << (camera * pos);

        // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)0);
        glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)0, PIXEL_COUNT);
    }
}