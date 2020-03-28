#pragma once

#include "gpu/opengl/gl.h"
#include "primitive/math/vec2.h"
#include "primitive/math/vec3.h"

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
            SmartGLBuffer buffer;

          protected:
            vec3 surface_points[4] = {vec3(-1.0, 1.0, 0.0), vec3(1.0, 1.0, 0.0),
                                      vec3(1.0, -1.0, 0.0), vec3(-1.0, -1.0, 0.0)};


          public:
            Quadrilateral()
            {
                program = createShaderProgram(
                    "#version 430\n in vec3 vertex; void main(){ gl_Position = "
                    "vec4(vertex, 1.0); }",
                    "#version 430\n out vec4 FragColor; void main(){ FragColor = "
                    "vec4(1.0,0.1,0.5,1.0);}");
            }

            void draw()
            {
                //buffer.use(SKGLB::ARRAY_BUFFER);
                buffer.setData(&surface_points, 4 * 12);

                program.use();

                auto vecAttribute = program.getAttribute("vertex");

                vecAttribute.use(buffer, GL_FLOAT, 0, 0);

                glDrawArrays(GL_TRIANGLES, 0, 3);
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