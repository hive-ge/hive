#pragma once

#include <vector>

#include "gpu/gpu_primitive.hpp"
#include "gpu/opengl/program.h"
#include "primitive/boss.hpp"
#include "primitive/drone/drone.h"
#include "primitive/math/vec2.h"

#include "graphic/sprite_boss.hpp"
#include "graphic/texture_boss.hpp"

namespace hive
{
    using namespace math;

    struct ProgramProp : Prop {

      public:
        static const ushort TYPE = PROGRAM_PROP_TYPE;

      public:
        ProgramProp() : Prop(TYPE, sizeof(ProgramProp)) {}
    };

    /**
     * Handles the management and rendering of sprites using instanced render passes.
     */
    class DrawBoss : public Boss
    {
      public:
        static const unsigned IDENTIFIER = BOSS_IDENTIFIER_DRAW;

      public:
        DrawBoss() : Boss(IDENTIFIER) {}

      private:
      public:
        virtual ProgramProp * compileBasicProgram(std::string vert_shader, std::string frag_shader);
        // Called by BigBadBoss instance.
        virtual void update(float step){

            // For each actve GPU program do:
            //      initialize program
            //      load program data
            //      send program to GPU for rendering.

        };

        // Setup of boss systems after everything is loaded
        virtual void setup(){};

        // Called before exit
        virtual void teardown(){};

        // Called periodicolly to update priority values
        virtual int priority() { return 1; };

        void allowDraw(const Drone & drone);
    };

    ProgramProp * DrawBoss::compileBasicProgram(std::string vert_shader, std::string frag_shader)
    {
        ProgramDefinition def;

        def.vert = &vert_shader;
        def.frag = &frag_shader;

        Program p = createShaderProgram(def);

        ProgramProp * prop = new ProgramProp;

        return prop;
    };

    void DrawBoss::allowDraw(const Drone & drone)
    {
        // Look for drawing systems on drone
        std::vector<TextureProp *> textures;
        std::vector<SpriteProp *> sprites;
        // vector<MeshProp> meshes;

        auto prop = drone.props;

        while (prop) {

            switch (prop->type) {

            case SPRITE_PROP_TYPE:
                sprites.push_back(static_cast<SpriteProp *>(prop));
                break;
            case TEXTURE_PROP_TYPE:
                textures.push_back(static_cast<TextureProp *>(prop));
                break;
            case PROGRAM_PROP_TYPE:

                break;
            }

            prop = prop->next;
        };
    } // namespace hive