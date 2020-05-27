#include "include/hive.h"

#include <cstring>
#include <vector>

namespace hive
{


    struct CachedRenderableObject {
        GLint program          = 0;
        GLuint vao             = 0;
        GLuint vert_data       = 0;
        GLuint indice_data     = 0;
        unsigned element_count = 0;
    };

    static std::vector<CachedRenderableObject> render_cache;

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

                    const ShaderArtifact *vert = nullptr, *normal = nullptr, *uv = nullptr;

                    for (auto & artifact : program->getInputMap()) {

                        if (artifact.type == ShaderArtifact::ArtifactType::VertexData) {

                            print artifact;

                            switch (artifact.name_hash) {
                            case StringHash64("vert"):
                            case StringHash64("Vert"):
                            case StringHash64("vertex"):
                            case StringHash64("Vertex"):
                            case StringHash64("Pos"):
                            case StringHash64("pos"):
                            case StringHash64("position"):
                            case StringHash64("Position"):
                                vert = &artifact;
                                break;
                            case StringHash64("UV"):
                            case StringHash64("uv"):
                                uv = &artifact;
                                break;
                            case StringHash64("norm"):
                            case StringHash64("normal"):
                                normal = &artifact;
                                break;
                            }
                        }
                    }

                    unsigned element_count = mesh->numberOfVertices();

                    unsigned buffer_size = (vert ? 3 * 4 * element_count : 0) +
                                           (uv ? 2 * 4 * element_count : 0) +
                                           (normal ? 3 * 4 * element_count : 0);

                    unsigned stride_size = (vert ? 0 : 0) + (uv ? 0 : 0) + (normal ? 0 : 0);

                    // Create a buffer for this data.
                    GLuint vao, vbo = 0, vbo_i = 0;

                    glGenVertexArrays(1, &vao);

                    glBindVertexArray(vao);

                    glGenBuffers(1, &vbo);

                    glGenBuffers(1, &vbo_i);

                    glBindBuffer(GL_ARRAY_BUFFER, vbo);

                    char * buffer = new char[buffer_size];

                    const NativeMeshData & mesh_data = *(mesh->data->native);

                    print mesh_data.verts[0], mesh_data.verts[1], mesh_data.verts[2],
                        mesh_data.verts[3];

                    print mesh_data.faces[0], mesh_data.faces[1];

                    // Fill the buffer up - Move this to mesh fill function.
                    for (int i = 0, j = 0; i < element_count; i += 1) {

                        if (vert) {
                            std::memcpy(buffer + j, &(mesh_data.verts[i]), 12);
                            j += 12;
                        };

                        if (uv) {
                            std::memcpy(buffer + j, &mesh_data.UVs[i], 8);
                            j += 8;
                        };

                        if (normal) {
                            std::memcpy(buffer + j, &mesh_data.verts[i], 12);
                            j += 12;
                        };
                    }

                    glBufferData(GL_ARRAY_BUFFER, buffer_size, buffer, GL_STATIC_DRAW);

                    delete[] buffer;


                    // Setup attrib pointers.

                    unsigned long long offset = 0;

                    print stride_size;

                    if (vert) {
                        glEnableVertexAttribArray(vert->shader_location);
                        glVertexAttribPointer(vert->shader_location, 3, GL_FLOAT, GL_FALSE,
                                              stride_size, (void *)offset);
                        offset += 12;
                    }

                    if (uv) {
                        glEnableVertexAttribArray(uv->shader_location);
                        glVertexAttribPointer(uv->shader_location, 2, GL_FLOAT, GL_FALSE,
                                              stride_size, (void *)offset);
                        offset += 8;
                    }

                    if (normal) {
                        glEnableVertexAttribArray(normal->shader_location);
                        glVertexAttribPointer(normal->shader_location, 3, GL_FLOAT, GL_FALSE,
                                              stride_size, (void *)offset);
                        offset += 12;
                    }

                    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_i);

                    unsigned indice_count = mesh_data.faces.size();

                    unsigned indice_buffer_size = indice_count * sizeof(vec3i);

                    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indice_buffer_size,
                                 mesh_data.faces.data(), GL_STATIC_DRAW);

                    glBindVertexArray(0);

                    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

                    glBindBuffer(GL_ARRAY_BUFFER, 0);


                    CachedRenderableObject obj = {program->data->program, vao, vbo, vbo_i,
                                                  indice_count * 3};

                    render_cache.push_back(obj);
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

        for (auto renderable : render_cache) {

            glUseProgram(renderable.program);

            glBindVertexArray(renderable.vao);

            // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderable.indice_data);

            glDrawElements(GL_TRIANGLES, renderable.element_count, GL_UNSIGNED_INT, 0);
        }

        glBindVertexArray(0);

        // For render objects
        // If there
    };

    void RenderBoss::teardown(){

    };

} // namespace hive