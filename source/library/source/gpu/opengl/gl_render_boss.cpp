#include "include/hive.h"

#include <cstring>
#include <vector>

#include <GL/glew.h>
/* intentional */
#include <GL/gl.h>

namespace hive
{

    struct TextureBinding {
        unsigned short texture;
        unsigned short unit;
        unsigned short slot;
    };

    struct UniformBinding {
        void * uniform_data;
        GLuint uniform_type;
        GLint uniform_loc;
    };

    /*
     */
    struct CachedRenderableObject {
        GLint program          = 0;
        GLuint vao             = 0;
        GLuint vert_data       = 0;
        GLuint indice_data     = 0;
        unsigned element_count = 0;

        /* This should be rebuilt if any Texture PROPS are changed on the object. */
        TextureBinding texture;

        /* This should be rebuilt if any general vector PROPS are changed on the object. */
        std::vector<UniformBinding> uniforms;
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

        auto from = DroneDataPool::begin<Drone>();
        auto to   = DroneDataPool::end<Drone>();
        // Check the drones for update render settings.

        // Need drones that at least have have -> Render + Program

        for (auto it = from; it != to; it++) {

            Drone::Ref drone = *it;

            // Candidate for threading using a work queueing system.

            if (drone->flag != DRONE_FLAG_NEED_RENDER_UPDATE) continue;

            drone->flag = DRONE_FLAG_NEED_RENDER_UPDATE;

            if (drone->flag == DRONE_FLAG_CAN_RENDER) {

                Prop::Ref prop = DroneDataPool::getNextRef(drone);

                MeshProp::Ref mesh             = 0;
                ShaderProgramProp::Ref program = 0;
                std::vector<ImageProp::Ref> images;
                std::vector<FloatProp::Ref> floats;

                while (prop) {

                    if (prop.is<ShaderProgramProp>()) program = prop;

                    if (prop.is<MeshProp>()) mesh = prop;

                    if (prop.is<ImageProp>()) images.push_back(prop);

                    if (prop.is<FloatProp>()) floats.push_back(prop);

                    prop = DroneDataPool::getNextRef(prop);
                }

                if (mesh && program && mesh->data) {
                    // go through attributes and find relavent entries

                    const ShaderArtifact *vert = nullptr, *normal = nullptr, *uv = nullptr;

                    std::vector<const ShaderArtifact *> uniforms;

                    std::vector<const ShaderArtifact *> textures;

                    for (auto & artifact : program->getInputMap()) {

                        if (artifact.type == ShaderArtifact::ArtifactType::VertexData) {

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

                        if (artifact.type == ShaderArtifact::ArtifactType::Uniform)
                            uniforms.push_back(&artifact);

                        if (artifact.type == ShaderArtifact::ArtifactType::Texture1D ||
                            artifact.type == ShaderArtifact::ArtifactType::Texture2D ||
                            artifact.type == ShaderArtifact::ArtifactType::Texture3D ||
                            artifact.type == ShaderArtifact::ArtifactType::TextureCube) {
                            textures.push_back(&artifact);
                        }
                    }

                    CachedRenderableObject obj;

                    /**
                     * Setup Textures
                     */
                    if (textures.size() > 0) {

                        unsigned slot = 0;

                        for (auto texture_artifact : textures) {

                            StringHash64 name = texture_artifact->name_hash;

                            for (auto image : images) {

                                if (image->getTagHash() == name) {
                                    // Make sure the texture is uploaded to the GPU.
                                    image->uploadToVRAM();

                                    unsigned texture_point = image->data->gpu_handle;

                                    obj.texture.texture = texture_point;
                                    obj.texture.unit    = texture_artifact->shader_location;
                                    obj.texture.slot    = slot;

                                    slot++;
                                }
                            }
                        }
                    }


                    /**
                     * Setup Uniforms
                     *
                     * TODO Fill out with the rest of the uniform types
                     * currently only doing floats.
                     */
                    if (uniforms.size() > 0) {

                        for (auto uniform_artifact : uniforms) {


                            StringHash64 name = uniform_artifact->name_hash;

                            for (auto _float : floats) {

                                if (_float->getTagHash() == name) {
                                    // Make sure the texture is uploaded to the GPU.
                                    UniformBinding binding;

                                    binding.uniform_data = (void *)&((_float->data.raw()));

                                    binding.uniform_type = GL_FLOAT;

                                    binding.uniform_loc = uniform_artifact->shader_location;

                                    obj.uniforms.push_back(binding);
                                }
                            }
                        }
                    }

                    /**
                     *  Setup Vertex Uniforms.
                     */


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


                    obj.program       = program->data->program;
                    obj.vao           = vao;
                    obj.vert_data     = vbo;
                    obj.indice_data   = vbo_i;
                    obj.element_count = indice_count * 3;

                    render_cache.push_back(obj);
                };
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

            glUniform1i(renderable.texture.unit, 0);

            glActiveTexture(GL_TEXTURE0 + 0);

            glBindTexture(GL_TEXTURE_2D, renderable.texture.texture);

            // glActiveTexture(0);
            //
            // glBindTexture(GL_TEXTURE_2D, 0);

            for (auto uniform : renderable.uniforms) {

                float rot = *static_cast<float *>(uniform.uniform_data);

                glUniform1f(uniform.uniform_loc, rot);
            }

            glBindVertexArray(renderable.vao);

            glDrawElements(GL_TRIANGLES, renderable.element_count, GL_UNSIGNED_INT, 0);
        }

        glBindVertexArray(0);
    }; // namespace hive

    void RenderBoss::teardown(){

    };

} // namespace hive