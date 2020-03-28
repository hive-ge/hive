#pragma once

#include "gpu/opengl/gl.h"

#include "interface/input/keyboard/keyboard.hpp"

#include "primitive/log.h"

#include <iostream>

namespace hive
{
    namespace interface
    {

        enum SCREEN_MODE : char { FULLSCREEN = 0, WINDOWED = 1, MINIMIZED = 2 };

        /**
         *
         * A drawing surface encapsulating an OpenGL
         * render context and a host OS drawing field.
         *
         * The drawing surface may be a window or a
         * fullscreen viewport.
         *
         * TODO turn into an abstract class.
         */
        class Screen
        {
          public:
            /**
             * Width of the drawing surface in pixels.
             */
            unsigned int width = 0;

            /**
             * Height of the drawing surface in pixels.
             */
            unsigned int height = 0;

            /**
             * Bit depth of the drawing surface
             */
            unsigned int depth = 0;

            /**
             * The mode the screen is in. By default, a windowed
             * drawing surface will be created. by calling resize
             * with a different SCREEN_MODE, the Screen instance
             * will attempt to create a different drawing surface
             * with the required mode.
             */
            hive::interface::SCREEN_MODE mode = hive::interface::WINDOWED;

          public:
            Screen(unsigned w, unsigned h)
            {
                width  = w;
                height = h;
            };

            virtual ~Screen(){};

            /**
             * Initialize the drawing surface. Returns `false` if the
             * surface could not be initialized.
             */
            virtual bool init() = 0;

            /**
             * Initialize the drawing surface. Returns `false` if the
             * surface could not be initialized.
             */
            virtual bool update() = 0;

            /**
             * Resize the drawing surface. Returns `false` if the
             * surface could not be resized. Should clamp to actual
             * drawing surface size if beyond the physical dimensions.
             * if this is done, assume fullscreen is desired and toggle
             * display mode to FULLSCREEN
             */
            virtual bool resize(unsigned int width, unsigned int hight,
                                hive::interface::SCREEN_MODE mode = hive::interface::WINDOWED) = 0;
        };

    } // namespace interface
} // namespace hive


#ifdef HIVE_USE_GLFW

namespace hive
{
    namespace interface
    {

        class GLFWScreen : public hive::interface::Screen
        {

            keyboard::Keyboard keyboard;

            GLFWwindow * window = nullptr;

          public:
            GLFWScreen(unsigned int w = 1024, unsigned int h = 720) : Screen(w, h){};

            virtual ~GLFWScreen()
            {
                if (window != nullptr) glfwTerminate();
            };

            virtual bool init()
            {

                if (glfwInit()) {

                    window = glfwCreateWindow(width, height, "Hello World", NULL, NULL);

                    if (window == nullptr) {
                        glfwTerminate();
                        return false;
                    }

                    /*make this the current context */
                    glfwMakeContextCurrent(window);

                    /* Initialize glew to get GL extensions running */
                    if (glewInit() == GLEW_OK) return true;
                }

                return false;
            };

            virtual bool update()
            {

                /* Get keyboard events and update the keyboard object state */

                /* Render here */
                // glClear(GL_COLOR_BUFFER_BIT);

                /* Swap front and back buffers */
                glfwSwapBuffers(window);

                /* Poll for and process events */
                // glfwPollEvents();

                return true;
            };

            virtual bool resize(unsigned int w = 0, unsigned int h = 0,
                                hive::interface::SCREEN_MODE mode = hive::interface::WINDOWED)
            {
                return true;
            };

            hive::interface::keyboard::Keyboard & getKeyboard() { return keyboard; }
        };
    } // namespace interface
} // namespace hive

#endif