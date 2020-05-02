#pragma once

#include "primitive/drone.hpp"

#include "primitive/prop.hpp"
#include "primitive/typedef.hpp"

#include <iostream>
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
        static std::vector<Boss *> bosses;

      private:
        ubyte index = 0;

      protected:
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

            for (auto boss : bosses) static_cast<Boss *>(boss)->index = index++;
        }

      public:
        Boss(const unsigned _id = 0);

        virtual ~Boss()
        {
            std::cout << "PRE DELETE :: Bosses Address " << (unsigned long long)&Boss::bosses
                      << " Size: " << Boss::bosses.size() << std::endl;
            bosses.erase(bosses.begin() + index - 1);
            setIndex();
        }

      protected:
        static void fatalError(std::string & error_message) {}

        static void fatalError() { SHOULD_EXIT = true; }

        /** returns the boss with with given id or nullptr. */
        static Boss * getBoss(unsigned boss_id)
        {

            for (auto boss : bosses)
                if (static_cast<Boss *>(boss)->id == boss_id) return static_cast<Boss *>(boss);

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

        /**
         * Get a props data value.
         */
        template <class T> inline T * getPropData(Prop * prop) { return prop->getData<T>(); }

        /**
         * Update a props data value.
         */
        template <class T> void setPropData(Prop * prop, T * data) { prop->setData<T>(data); }
    };

    /**
     * Handles the start up and shutdown of all other bosses
     * plus the main runtime.
     */
    class BigBadBoss : Boss
    {
      public:
        static const unsigned IDENTIFIER = BOSS_IDENTIFIER_BBB;

        std::vector<DroneData *> drones;

      public:
        BigBadBoss() : Boss(IDENTIFIER)
        {
            std::cout << "Bosses Address " << (unsigned long long)&Boss::bosses
                      << " Size: " << Boss::bosses.size() << std::endl;
            Drone::setBoss(this);
        }

        virtual ~BigBadBoss() {}

      public:
        inline bool canRun() { return !SHOULD_EXIT; }
        bool update();
        virtual void setup();
        void prepareDroneUpdate(hive::Drone * drone) {}

        /*
          Creates new drone object and assignes a opaque drone pointer to it.
          DO NOT try manipulate drone pointers. They are not memory pointers
          and are used by bosses to handle storage of drone objects.
         */
        DroneData * createDroneData(Drone * drone);

        /*
          Returns reference to actual drone object. The address of this reference
          SHOULD NOT be stored for later retrieval.
        */
        DroneData * getDroneData(Drone *);

      protected:
        virtual void teardown();
        virtual int priority();
        virtual void update(float step);
    };
} // namespace hive