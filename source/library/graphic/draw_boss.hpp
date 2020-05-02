#pragma once

#include <vector>

#include "gpu/gpu_primitive.hpp"
#include "gpu/opengl/program.hpp"
#include "primitive/boss.hpp"
#include "primitive/drone/drone.hpp"
#include "primitive/math.hpp"

#include "graphic/sprite_boss.hpp"
#include "graphic/texture_boss.hpp"

namespace hive
{
    using namespace math;

    // Quadrilateral Surface points
    static unsigned int quad_indices[6] = {0, 1, 3, 3, 2, 1}; // 24

    // Quadrilateral UV coords
    static vec2 quad_uvs[4] = {{0.0, 0.0}, {1.0, 0.0}, {1.0, 1.0}, {0.0, 1.0}}; // 24

    // Quadrilateral Surface points
    static vec3 quad_vecs[4] = {{-1.0, 1.0, 0.0},
                                {1.0, 1.0, 0.0},
                                {1.0, -1.0, 0.0},
                                {-1.0, -1.0, 0.0}}; // 48

    struct ProgramProp : Prop {

      public:
        static const ushort TYPE = PROGRAM_PROP_TYPE;

        Program program;

      public:
        ProgramProp() : Prop(TYPE, sizeof(ProgramProp)) {}
    };

    struct DrawSequence {
        std::vector<TextureProp *> textures;
        std::vector<SpriteProp *> sprites;
        Program program;
        VRAMBuffer a;
        VRAMBuffer b;
        VRAMBuffer c;
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
        std::vector<DrawSequence> sequences;

      public:
        virtual ProgramProp * compileBasicProgram(std::string vert_shader, std::string frag_shader);
        // Called by BigBadBoss instance.
        virtual void update(float step);

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

        prop->program = p;

        return prop;
    };

    void DrawBoss::update(float step)
    {

        //Determin if any active drones need to be reconfigured.
          //If a drone has different rendering properties:
            //Determine if drone has appropriate render props:
              //Program(s)
              //Optional Pipeline information
              //Mesh Data
              //Uniform data as determined by program
                // vecs, matrices, and single values
              //Texture Data
            
            //Sprites combine a subset of the above for 2D billboard style rendering
            
            //If the drone has appropriate data:
              // Reorder drone data if necessary for more appropriate render passes
              // Sort render information for all drones for more appropriate render passes
              // 

            //If the drone does not have appropriate data:
              // Create a message of errors and assigne to drone.
              // Set the unrenderable flag on drone. 

        // Once all drones that have RENDER_PREP cleared:
          // Start with the left most drone, setup its render
          // systems, and render the drone. 
          
          // Optional, use culling system to determin if a node should be rendered or not.

          //Pipelines and SubPipelines determine the global attributes such as camera orientation.

          // Pipelines may seperate different passes, or need to rely on information
          // That has been created in other passes. These are ordered depending on it's
          // dependencies and its dependents. 
        


        // For each actve GPU program do:
        //      initialize program
        //      load program data
        //      send program to GPU for rendering.
        for (auto & seq : sequences) {

            seq.program.use();


            seq.c.use(SKGLB::ELEMENT_ARRAY_BUFFER);

            for (auto tex : seq.textures) tex->useTexture();

            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)0);
        }
    }

    void DrawBoss::allowDraw(const Drone & drone)
    {
        // Look for drawing systems on drone
        std::vector<TextureProp *> textures;
        std::vector<SpriteProp *> sprites;
        std::vector<ProgramProp *> programs;
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
                programs.push_back(static_cast<ProgramProp *>(prop));
                break;
            }

            prop = prop->next;
        };

        // Treat as a sprite sheet.
        if (sprites.size() > 0) {

            // Element instance are used when attribs are the same but uniforms are different

            SpriteProp & sprite   = *sprites[0];
            TextureProp & texture = *textures[0];
            ProgramProp & program = *programs[0];


            DrawSequence seq;

            seq.sprites  = sprites;
            seq.textures = textures;
            seq.program  = program.program;

            program.program.use();

            texture.useTexture();

            auto vert = program.program.getInput("vert");
            auto UV   = program.program.getInput("UVs");

            VRAMBuffer a;
            VRAMBuffer b;
            VRAMBuffer c;

            a.setData(&quad_vecs, 48);
            b.setData(&quad_uvs, 32);
            c.setData(&quad_indices, 24);

            vert.use(a, GL_FLOAT, 0, 0, 0, false);
            UV.use(b, GL_FLOAT, 0, 0, 0, false);

            seq.a = a;
            seq.b = b;
            seq.c = c;

            // c.use(SKGLB::ELEMENT_ARRAY_BUFFER);

            sequences.push_back(seq);

            // program.program.release();

            // Straight element draw


            // texture.bind(i);
        }
    }
} // namespace hive