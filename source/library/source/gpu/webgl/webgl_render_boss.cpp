#include "include/hive.h"

#include <vector>

namespace hive
{

    RenderBoss::RenderBoss()
        : Boss(IDENTIFIER){

          };

    RenderBoss::~RenderBoss(){

    };

    void RenderBoss::onMessage(StringHash64 message_id, const char * message_data,
                               const unsigned message_length)
    {
    }

    void RenderBoss::setup(){

    };

    struct CachedRenderableObject {
        GLint program = 0;
        GLint vao     = 0;
        GLint vbo     = 0;
    };

    void RenderBoss::update(float)
    {
        // Check the drones for update render settings.
        for (auto & drone : drones) {

            // Candidate for threading using a work queueing system.

            if (drone->flag != DRONE_FLAG_NEED_RENDER_UPDATE) continue;

            drone->flag ^= DRONE_FLAG_NEED_RENDER_UPDATE;

            if (drone->flag == DRONE_FLAG_CAN_RENDER) {


                auto * prop = drone->props;

                MeshProp * mesh = nullptr;

                ShaderProgramProp * program = nullptr;

                while (prop != nullptr) {

                    if (prop->type == StringHash64("PROP_GPU_PROGRAM"))
                        program = static_cast<ShaderProgramProp *>(prop);

                    if (prop->type == StringHash64("PROP_MESH"))
                        mesh = static_cast<MeshProp *>(prop);

                    prop = prop->next;
                }

                if (mesh && program && mesh->data) {
                    // go through attributes and find relavent entries

                    for (auto & artifact : program->getInputMap()) {
                        if (artifact.type == ShaderArtifact::ArtifactType::DataInput) {


                            switch (artifact.name_hash) {
                            case StringHash64("vert"):
                            case StringHash64("vertex"):
                                std::cout << "got vertex" << std::endl;
                                break;
                            case StringHash64("UV"):
                            case StringHash64("uv"):
                                std::cout << "got uv" << std::endl;
                                break;
                            case StringHash64("norm"):
                            case StringHash64("normal"):
                                std::cout << "got normal" << std::endl;
                                break;
                            }
                        }
                    }
                }
            }
        }

        /**
         *   On message update or intermittent interval, create render settings:
         *
         *      Meshes -> Materials -> Uniforms -> Batches
         *
         *   When ready to render render all batches.
         */

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // For render objects
        // If there
    };

    void RenderBoss::teardown(){

    };

} // namespace hive