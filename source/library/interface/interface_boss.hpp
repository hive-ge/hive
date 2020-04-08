#pragma once

#include "interface/input/controller/controller.hpp"
#include "interface/input/keyboard/keyboard.hpp"
#include "interface/input/mouse/mouse.hpp"
#include "primitive/boss.hpp"
#include "primitive/log.h"

#include <iostream>
#include <vector>

namespace hive
{
    namespace interface
    {

        enum SCREEN_MODE : char { FULLSCREEN = 0, WINDOWED = 1, MINIMIZED = 2 };

        /**
         *
         * A wrapper around interface systems to provide
         * basic eventing, windowing, and input.
         *
         */
        class InterfaceBoss : public Boss
        {

          public:
            static const unsigned IDENTIFIER = BOSS_IDENTIFIER_INTERFACE;

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
            InterfaceBoss(unsigned w, unsigned h) : Boss(IDENTIFIER)
            {
                width  = w;
                height = h;
            };

            virtual ~InterfaceBoss(){};

            /**
             * Resize the drawing surface. Returns `false` if the
             * surface could not be resized. Should clamp to actual
             * drawing surface size if beyond the physical dimensions.
             * if this is done, assume fullscreen is desired and toggle
             * display mode to FULLSCREEN
             */
            virtual bool resize(unsigned int width, unsigned int hight,
                                hive::interface::SCREEN_MODE mode = hive::interface::WINDOWED) = 0;

            /**
             * Returns a single Keyboard::KeyStateReader object.
             * This is always the same object for each call to this function.
             */
            virtual hive::interface::keyboard::Keyboard::KeyStateReader & getKeyboard() = 0;

            virtual hive::interface::controller::Controller::ControllerStateReader &
            getController(unsigned index) = 0;

            virtual hive::interface::mouse::Mouse::MouseStateReader & getMouse() = 0;
        };

    } // namespace interface
} // namespace hive
