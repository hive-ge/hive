#pragma once

#include "primitive/drone/drone.h"
#include "primitive/typedef.h"
#include <string>
#include <vector>


#define BOSS_IDENTIFIER_NULL 0
#define BOSS_IDENTIFIER_BBB 1
#define BOSS_IDENTIFIER_DRAW 2
#define BOSS_IDENTIFIER_SPRITE 3
#define BOSS_IDENTIFIER_TEXTURE 4
#define BOSS_IDENTIFIER_INTERFACE 5

namespace hive
{
    // Forward declare the big one.
    class BigBadBoss;

    class Boss
    {
        friend BigBadBoss;

      public:
        static const unsigned IDENTIFIER = BOSS_IDENTIFIER_NULL;

      private:
        ubyte index = 0;

      protected:
        static std::vector<Boss *> bosses;

        const unsigned id = 0;

        /***
         * Global Boolean indicating whether the
         * application should quit. Use in conjuction
         * with fatalError or prepareExit commands
         */
        static bool SHOULD_EXIT;

      private:
        /**
         * Update  bosses index property.
         */
        static void setIndex()
        {
            ubyte index = 1;

            for (auto boss : bosses) boss->index = index++;
        }

      public:
        Boss(const unsigned _id = 0) : id(_id)
        {
            bosses.push_back(this);

            setIndex();
        }

        virtual ~Boss()
        {
            bosses.erase(bosses.begin() + index - 1);
            setIndex();
        }

      protected:
        static void fatalError(std::string & error_message) {}

        static void fatalError() { SHOULD_EXIT = true; }

        /** returns the boss with with given id or nullptr. */
        static Boss * getBoss(unsigned boss_id)
        {

            for (auto boss : bosses) {
                if (boss->id == boss_id) return boss;
            }

            return nullptr;
        };

        void prepareExit() { SHOULD_EXIT = true; }

      protected:
        // Called by BigBadBoss instance.
        virtual void update(float step) = 0;

        // Setup of data
        virtual void setup() = 0;

        // Called before exit
        virtual void teardown() = 0;

        // Called periodicolly to update priority values
        virtual int priority() = 0;
    };

    bool Boss::SHOULD_EXIT = false;

    std::vector<Boss *> Boss::bosses;

    /**
     * Handles the start up and shutdown of all other bosses
     * plus the main runtime.
     */
    class BigBadBoss : Boss
    {
      public:
        static const unsigned IDENTIFIER = BOSS_IDENTIFIER_BBB;

        std::vector<Drone *> drones;

      public:
        BigBadBoss() : Boss(IDENTIFIER) {}

        virtual ~BigBadBoss() {}

      public:
        inline bool canRun() { return !SHOULD_EXIT; }
        bool update();
        virtual void setup();
        void prepareDroneUpdate(hive::Drone * drone) {}

        inline Drone & createDrone()
        {
            hive::Drone & drone = *new Drone(this);

            drone.id.id = drones.size();

            drones.push_back(&drone);

            return drone;
        }

      protected:
        virtual void teardown();
        virtual int priority();
        virtual void update(float step);
    };

    void BigBadBoss::setup()
    {
        for (auto boss : bosses) {

            if (boss == this) continue;

            boss->setup();
        }
    };

    void BigBadBoss::teardown(){

    };

    int BigBadBoss::priority() { return 0; };


    void BigBadBoss::update(float delta_t)
    {
        if (SHOULD_EXIT) {

            for (auto boss : bosses) {

                if (boss == this) continue;

                boss->teardown();
            }

            teardown();

        } else {
            for (auto boss : bosses) {

                if (boss == this) continue;

                boss->update(delta_t);
            }
        }
    }

    bool BigBadBoss::update()
    {

        update(0.0);

        if (SHOULD_EXIT) {

            for (auto boss : bosses) {

                if (boss == this) continue;

                boss->teardown();
            }

            teardown();

            return false;
        }

        return true;
    }


    void hive::Drone::signalUpdate()
    {
        if (flags) boss->prepareDroneUpdate(this);
    }

} // namespace hive