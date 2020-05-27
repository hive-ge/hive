#pragma once

#include "include/boss/boss.hpp"
#include <vector>

namespace hive
{
    class RenderBoss : public Boss
    {

      public:
        static const unsigned IDENTIFIER = BOSS_IDENTIFIER_DRAW;

      public:
        /**
         * The mode the screen is in. By default, a windowed
         * drawing surface will be created. by calling resize
         * with a different SCREEN_MODE, the Screen instance
         * will attempt to create a different drawing surface
         * with the required mode.
         */

      public:
        RenderBoss();

        virtual ~RenderBoss();

      protected:
        virtual int priority() { return 2; }

        virtual void setup();

        virtual void update(float);

        virtual void teardown();

        virtual void onMessage(StringHash64 message_id, const char * message_data = nullptr,
                               const unsigned message_length = 0);
    };
} // namespace hive