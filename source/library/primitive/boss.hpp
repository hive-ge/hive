#pragma once

#include "./drone/drone.hpp"

#include "./typedef.hpp"

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

    /**
     * Handles the start up and shutdown of all other bosses
     * plus the main runtime.
     */
    class BigBadBoss : public Boss
    {
      public:
        static const unsigned IDENTIFIER = BOSS_IDENTIFIER_BBB;

        std::vector<Drone *> drones;

      public:
        BigBadBoss() : Boss(IDENTIFIER) { Drone::setBoss(this); }

        virtual ~BigBadBoss() {}

      public:
        inline bool canRun() { return !SHOULD_EXIT; }
        bool update();
        virtual void setup();
        void prepareDroneUpdate(hive::Drone * drone) {}

        inline Drone * createDrone()
        {
            BigBadBoss & boss = *static_cast<BigBadBoss *>(Drone::getBoss());

            hive::Drone & drone = *new Drone();

            drone.id.id = drones.size();

            drones.push_back(&drone);

            return (Drone *)drone.id.id;
        }

        inline Drone & getDrone(unsigned id) { return *drones[id]; }

      protected:
        virtual void teardown();
        virtual int priority();
        virtual void update(float step);
    };
} // namespace hive