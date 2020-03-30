#pragma once

#include "gpu/opengl/gl.h"
#include "primitive/math/vec2.h"
#include "primitive/math/vec3.h"
#include "resource/load.hpp"

#include <GL/gl.h>
#include <iostream>
#include <math.h>
#include <stdio.h>

#include <vector>

#define pi 3.142857

#define HIVEcreateBITFIELDMethods(ENUMNAME, FIELDNAME)                                             \
  protected:                                                                                       \
    inline void set##FIELDNAME(bool state = 1)                                                     \
    {                                                                                              \
        BITFIELD = (BITFIELD & ~ENUMNAME::FIELDNAME) | (state & ENUMNAME::FIELDNAME);              \
    };                                                                                             \
                                                                                                   \
  public:                                                                                          \
    inline bool is##FIELDNAME() { return BITFIELD & ENUMNAME::FIELDNAME; };

namespace hive
{
    namespace graphic
    {

        using namespace hive::math;
        using namespace hive::gl;


        /**
         * Draw Objects can internal bit flags set
         * to specific values to indicate drawing
         * state of the object. These flags can be
         * indexed though the values of this enum.
         */
        enum DRAW_BITFIELD_FLAGS : const unsigned {
            VISIBLE       = 0x1,
            TRANSLUESCENT = 0x2,
            STATIC        = 0x4
        };

        /**
         * A simple graphic primitive that represents
         * a 3D transformation and a general drawing
         * routine.
         *
         * SHOULD be inherited and expanded into a
         * more useful graphic object.
         */
        class DrawObject
        {
          private:
            /**
             *
             */
            unsigned BITFIELD = 0;

          protected:
            hive::math::vec3 scale;
            hive::math::vec3 position;
            hive::math::vec3 rotation;

            HIVEcreateBITFIELDMethods(DRAW_BITFIELD_FLAGS, VISIBLE);
            HIVEcreateBITFIELDMethods(DRAW_BITFIELD_FLAGS, TRANSLUESCENT);
            HIVEcreateBITFIELDMethods(DRAW_BITFIELD_FLAGS, STATIC);

          public:
            DrawObject(){};

            ~DrawObject(){};
            /**
             * Initializes drawing routine
             *
             * Depending on implementation, drawing
             * may be deferred to handle
             * composition and optimization.
             */
            virtual void draw();
        };

        class Quadrilateral : public DrawObject
        {
            SmartGLProgram program;
            SmartGLBuffer bufferA;
            SmartGLBuffer bufferB;
            SmartGLUniformBlock obj_data;
            SmartGLUniform camera;

            unsigned int indices[6] = {3, 1, 0, 3, 2, 1};

            vec3 surface_points[4] = {{-1.0, 1.0, 0.0},
                                      {1.0, 1.0, 0.0},
                                      {1.0, -1.0, 0.0},
                                      {-1.0, -1.0, 0.0}};

          public:
            vec3 pos = {0.0, 0.0, 0.0};
            vec3 rot = {0.0, 0.0, 0.0};
            vec3 scl = {1.0, 1.0, 1.0};

          private:
          public:
            Quadrilateral()
            {
                if (!program.IS_USABLE()) {

                    program = createShaderProgram(
                        hive::resource::loadUTF8File(
                            "/home/anthony/work/active/apps/hive/source/shader/box.vert"),

                        hive::resource::loadUTF8File(
                            "/home/anthony/work/active/apps/hive/source/shader/box.frag"));

                    program.use();

                    auto vecAttribute = program.getInput("vertex");

                    obj_data = program.getUniformBlock("ObjData");


                    bufferA.setData(&indices, 24 + 48);

                    vecAttribute.use(bufferA, GL_FLOAT, 0, 24);

                    bufferA.use(SKGLB::ELEMENT_ARRAY_BUFFER);
                    camera = program.getUniform("camera");

                    // obj_data.bind(0);

                    camera.use();

                    // buffer.setData(&indices, 36 + 48 + 24);

                    // buffer.useRange(hive::gl::SKGLB::UNIFORM_BUFFER, 0, 24 + 48, 36);
                    program.release();
                    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
                }
            }

            void draw(mat44 projection_view)
            {

                clearErrors();

                program.use();

                camera << &projection_view;

                // buffer.use(SKGLB::ARRAY_BUFFER);
                // return;
                glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)0);


                GLenum err = glGetError();

                switch (err) {
                case GL_INVALID_ENUM:
                    // is generated by glBufferData if target is not one of the
                    // accepted buffer targets.

                    // is generated if usage is not GL_STREAM_DRAW, GL_STREAM_READ,
                    // GL_STREAM_COPY, GL_STATIC_DRAW, GL_STATIC_READ, GL_STATIC_COPY,
                    // GL_DYNAMIC_DRAW, GL_DYNAMIC_READ, or GL_DYNAMIC_COPY.

                    __ERROR("Buffer target not valid.");
                    throw - 1;


                case GL_INVALID_VALUE:
                    // is generated if size is negative.

                    __ERROR("Size of buffer cannot be negative");
                    throw - 1;

                case GL_INVALID_OPERATION:
                    // is generated by glBufferData if the reserved buffer object
                    // name 0 is bound to target.

                    // is generated by glNamedBufferData if buffer is not the
                    // name of an existing buffer object.

                    // is generated if the GL_BUFFER_IMMUTABLE_STORAGE flag of
                    // the buffer object is GL_TRUE.

                    __ERROR("Invalid use of buffer object: Either buffer name is 0, buffer name\n"
                            "does not exist, or buffer is immutable.");
                    throw - 1;

                case GL_OUT_OF_MEMORY:
                    // is generated if the GL is unable to create a data store with
                    // the specified size.

                    __ERROR("Size of buffer cannot be negative");
                    throw - 1;
                }
            }
        };

        //################################################################
        //################################################################
        //                 DEFAULT DRAWING IMPLEMENTATION
        //################################################################
        //################################################################

        void DrawObject::draw(){


        };


    } // namespace graphic
} // namespace hive