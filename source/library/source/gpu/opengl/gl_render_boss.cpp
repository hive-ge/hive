#include "include/hive.h"

#include <GL/glew.h>
#include <array>
#include <cstring>
#include <functional>

#include <vector>
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
        Drone::Ref drone       = 0;
        bool uses_scene_camera = false;
        GLint program          = 0;
        GLuint vao             = 0;
        GLuint vert_data       = 0;
        GLuint indice_data     = 0;

        unsigned element_count = 0;

        /* This should be rebuilt if any Texture PROPS are changed on the object. */
        TextureBinding texture;

        /* This should be rebuilt if any general vector PROPS are changed on the object. */
        std::vector<UniformBinding> uniforms;
        std::vector<TextureBinding> textures;
    };

    struct CachedRenderPass {

        Drone::Ref drone    = 0;
        char id             = -1;
        unsigned char order = 0;
        bool active         = true;

        // Camera data for the pass
        void * camera_data = nullptr;

        // Secondary Ordering (after textures)
        // To determine render order of elements.
        GLint frame_buffer = 0;

        // Update camera here.
        // std::vector<UniformBinding> global_uniforms;
        std::vector<CachedRenderableObject> objects;

        // Used to determine mapping between passes.
        // Also used to detect when textures are overlapping.
        std::vector<GLuint> input_textures;
        std::vector<GLuint> output_textures;

        CachedRenderPass * next = nullptr;
    };

    struct RenderPass {
    };

    static std::vector<CachedRenderPass> render_passes;

    RenderBoss::RenderBoss()
        : Boss(IDENTIFIER){

          };

    RenderBoss::~RenderBoss(){

    };

    void setupCamera(Drone::Ref drone, DoubleMat44Prop::Ref camera)
    {
        // Extract data and setup uniform. Append the data to the render pass.
    }


    // Camera, Lights, etc.
    void setupUniform(Drone::Ref & drone, CachedRenderPass & pass)
    {
        if (drone->flag != DRONE_FLAG_NEED_RENDER_UPDATE) return;

        drone->flag -= DRONE_FLAG_NEED_RENDER_UPDATE;
        // Check for any matrix or vector objects that can be used for rendering.

        if (drone->getCache() == Mat44FloatProp::DroneDataType) {
            // use matrices for camera
            Mat44FloatProp::Ref mat = drone->getFirstPropOfType<Mat44FloatProp>();

            if (mat->getTagHash() == StringHash64("camera") ||
                mat->getTagHash() == StringHash64("Camera")) {
                pass.camera_data = (void *)(&mat->data.raw());
            }
        }
    };


    // Called by other functions.
    void setupTexture(ImageProp::Ref & image)
    {
        auto data = image->data; //<- this image data becomes a buffer data.

        if (data->gpu_handle) return; // already have a texture present on the gpu

        // The texture we're going to render to
        GLuint renderedTexture;

        glGenTextures(1, &renderedTexture);

        glBindTexture(GL_TEXTURE_2D, renderedTexture);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, data->width, data->height, 0, GL_RGB,
                     GL_UNSIGNED_BYTE, 0);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

        data->gpu_handle = renderedTexture;

        data.update();
    }


    // Must be contain mesh data. All else is optional.
    void setupObject(Drone::Ref & drone, CachedRenderPass & pass)
    {

        /**
         * Need to determine the folling kind of drones:
         *  Objects: Anything with mesh data.
         *  Uniforms: Anything with matching uniform tags.
         *      Lights - Array of lights. Light tag - generic tags.
         *  Cameras: Anything with matching tag.
         *  Render Buffer Output:
         *      Determines camera
         *
         *  Sort Render passes.
         */

        // Candidate for threading using a work queueing system.
        bool USES_PASS_CAMERA = false;

        if (drone->flag != DRONE_FLAG_NEED_RENDER_UPDATE) return;

        drone->flag -= DRONE_FLAG_NEED_RENDER_UPDATE;

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
                // go through attributes and find relevent entries

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
                                setupTexture(image);

                                unsigned texture_handle = image->data->gpu_handle;

                                obj.texture.texture = texture_handle;
                                obj.texture.unit    = texture_artifact->shader_location;
                                obj.texture.slot    = slot;

                                pass.input_textures.push_back(texture_handle);

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

                        if (name == StringHash64("Camera") || name == StringHash64("camera")) {

                            UniformBinding binding;

                            binding.uniform_data = nullptr;

                            binding.uniform_type = 11551155;

                            binding.uniform_loc = uniform_artifact->shader_location;

                            obj.uniforms.push_back(binding);

                        } else {

                            for (auto _float : floats) {


                                if (_float->getTagHash() == name) {

                                    UniformBinding binding;

                                    binding.uniform_data = (void *)&((_float->data.raw()));

                                    binding.uniform_type = GL_FLOAT;

                                    binding.uniform_loc = uniform_artifact->shader_location;

                                    obj.uniforms.push_back(binding);
                                }
                            }
                        }
                    }
                }

                /**
                 *  Setup Vertex Uniforms.
                 */


                unsigned element_count = mesh->numberOfVertices();

                unsigned stride_size = 0;

                unsigned buffer_size = (vert ? 3 * 4 * element_count : 0) +
                                       (uv ? 2 * 4 * element_count : 0) +
                                       (normal ? 3 * 4 * element_count : 0);
                unsigned stride_marker = 0;

                if (vert) {
                    buffer_size += 3 * 4 * element_count;
                    stride_size += 12;
                    stride_marker++;
                }

                if (uv) {
                    buffer_size += 2 * 4 * element_count;
                    stride_size += 8;
                    stride_marker++;
                }

                if (normal) {
                    buffer_size += 2 * 4 * element_count;
                    stride_size += 12;
                    stride_marker++;
                }

                if (stride_marker < 1) stride_size = 0;

                // If more than one element is selected the

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

                unsigned long long offset = 0;

                if (vert) {
                    glEnableVertexAttribArray(vert->shader_location);
                    glVertexAttribPointer(vert->shader_location, 3, GL_FLOAT, GL_FALSE, stride_size,
                                          (void *)offset);
                    offset += 12;
                }

                if (uv) {
                    glEnableVertexAttribArray(uv->shader_location);
                    glVertexAttribPointer(uv->shader_location, 2, GL_FLOAT, GL_FALSE, stride_size,
                                          (void *)offset);
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

                glBufferData(GL_ELEMENT_ARRAY_BUFFER, indice_buffer_size, mesh_data.faces.data(),
                             GL_STATIC_DRAW);

                glBindVertexArray(0);

                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

                glBindBuffer(GL_ARRAY_BUFFER, 0);

                obj.program           = program->data->program;
                obj.vao               = vao;
                obj.vert_data         = vbo;
                obj.uses_scene_camera = USES_PASS_CAMERA;
                obj.indice_data       = vbo_i;
                obj.element_count     = indice_count * 3;

                pass.objects.push_back(obj);
            };
        }
    }
    // Determin the output. Should trow error if this is not present.
    void setupRenderBuffer(Drone::Ref & drone, CachedRenderPass & pass)
    {
        struct Render {
        };


        if (drone->flag != DRONE_FLAG_NEED_RENDER_UPDATE) return;

        drone->flag -= DRONE_FLAG_NEED_RENDER_UPDATE;
        // Setup a frame buffer.

        // Need to setup color attachments

        GLuint frame_buffer;

        glGenFramebuffers(1, &frame_buffer);

        glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer);

        // Only image props can be used for color attachments.

        DroneDataPool::TypeIterator<ImageProp> image_props;


        // Default attachments.
        bool HAS_COLOR = false, HAS_DEPTH = false, HAS_STENCIL = false;

        for (auto image : image_props) {

            // Image already exists skip this step.
            setupTexture(image);

            // Get image name and bind to color attachment

            // For each image, create a render buffer to receive the data.
            int index = 0;

            switch (image->getTagHash()) {
            case StringHash64("color_slot1"):
                index = 0;
                break;
            case StringHash64("color_slot2"):
                index = 1;
                break;
            case StringHash64("color_slot3"):
                index = 2;
                break;
            case StringHash64("color_slot4"):
                index = 3;
                break;
            case StringHash64("color_slot5"):
                index = 4;
                break;
            case StringHash64("color_slot6"):
                index = 5;
                break;
            case StringHash64("color_slot7"):
                index = 6;
                break;
            case StringHash64("depth_slot"):
                index = 256;
                break;
            case StringHash64("stencil_slot"):
                index = 128;
                break;
            default:
                index = 0;
                break;
            }

            if (index == 128) {
                HAS_STENCIL = true;
                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D,
                                       image->data->gpu_handle, 0);
            } else if (index == 256) {
                HAS_DEPTH = true;
                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D,
                                       image->data->gpu_handle, 0);
            } else {
                HAS_COLOR = true;

                // data.
                HIVE_DEBUG_WARN("Need error check for glGenRenderBuffer");

                HIVE_DEBUG_WARN(
                    "Do we need check to see if the output is correct for the display?");
                HIVE_DEBUG_WARN("Need error check for glRenderbufferStorage");

                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, GL_TEXTURE_2D,
                                       image->data->gpu_handle, 0);
            }

            pass.output_textures.push_back(image->data->gpu_handle);
        }

        // Need at most one depth texture.
        if (!HAS_DEPTH) {
            // If not depth Setup depth texture.
            GLuint depth_renderbuffer;

            glGenRenderbuffers(1, &depth_renderbuffer);

            glBindRenderbuffer(GL_RENDERBUFFER, depth_renderbuffer);

            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 1280, 720);

            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER,
                                      depth_renderbuffer);
        }

        // Need at least one color attachment.
        if (!HAS_COLOR) {
            // Set default color attachment
            GLuint color_render_buffer;

            glGenRenderbuffers(1, &color_render_buffer);

            glBindRenderbuffer(GL_RENDERBUFFER, color_render_buffer);

            glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA, 1280, 720);

            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER,
                                      color_render_buffer);
        }

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            HIVE_ERROR("Frame buffer incomplete");

        pass.frame_buffer = frame_buffer;

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    /** GL Buffer Internal formats

    Sized Internal Format	Base Format	    R Bit   G Bit   B Bits  A Bits

    GL_R8	                GL_RED	        8
    GL_R8UI	                GL_RED_INTEGER	ui8
    GL_R8I	                GL_RED_INTEGER	i8
    GL_R16UI	            GL_RED_INTEGER	ui16
    GL_R16I	                GL_RED_INTEGER	i16
    GL_R32UI	            GL_RED_INTEGER	ui32
    GL_R32I	                GL_RED_INTEGER	i32
    GL_RG8	                GL_RG	        8	    8
    GL_RG8UI	            GL_RG_INTEGER	ui8	    ui8
    GL_RG8I	                GL_RG_INTEGER	i8	    i8
    GL_RG16UI	            GL_RG_INTEGER	ui16    ui16
    GL_RG16I	            GL_RG_INTEGER	i16	    i16
    GL_RG32UI	            GL_RG_INTEGER	ui32    ui32
    GL_RG32I	            GL_RG_INTEGER	i32	    i32
    GL_RGB8	                GL_RGB	        8	    8	    8
    GL_RGB565	            GL_RGB	        5	    6	    5
    GL_RGBA8	            GL_RGBA	        8	    8	    8	    8
    GL_SRGB8_ALPHA8	        GL_RGBA	        8	    8	    8	    8
    GL_RGB5_A1	            GL_RGBA	        5	    5	    5	    1
    GL_RGBA4	            GL_RGBA	        4	    4	    4	    4
    GL_RGB10_A2	            GL_RGBA	        10	    10      10	    2
    GL_RGBA8UI	            GL_RGBA_INTEGER	ui8	    ui8	    ui8	    ui8
    GL_RGBA8I	            GL_RGBA_INTEGER	i8	    i8	    i8	    i8
    GL_RGB10_A2UI	        GL_RGBA_INTEGER	ui10	ui10	ui10	ui2
    GL_RGBA16UI	            GL_RGBA_INTEGER	ui16	ui16	ui16	ui16
    GL_RGBA16I	            GL_RGBA_INTEGER	i16	    i16	    i16	    i16
    GL_RGBA32I	            GL_RGBA_INTEGER	i32	    i32	    i32	    i32
    GL_RGBA32UI	            GL_RGBA_INTEGER	ui32	ui32	ui32	ui32

    Stencil

    Sized Internal Format	Base Format	        Depth Bits  Stencil Bits

    GL_DEPTH_COMPONENT16	GL_DEPTH_COMPONENT	16          0
    GL_DEPTH_COMPONENT24	GL_DEPTH_COMPONENT	24          0
    GL_DEPTH_COMPONENT32F	GL_DEPTH_COMPONENT	f32         0
    GL_DEPTH24_STENCIL8	    GL_DEPTH_STENCIL	24	        8
    GL_DEPTH32F_STENCIL8	GL_DEPTH_STENCIL	f32	        8
    GL_STENCIL_INDEX8	    GL_STENCIL	 	                8
    */

    // Compares two intervals according to staring times.
    bool compareDroneData(Drone::Ref & r1, Drone::Ref & r2)
    {

        // These are fiber-able.
        RenderableProp::Ref rp1 = r1->getFirstPropOfType<RenderableProp>();
        RenderableProp::Ref rp2 = r2->getFirstPropOfType<RenderableProp>();

        unsigned index1 = rp1->getRenderPassGroup();
        unsigned index2 = rp2->getRenderPassGroup();

        return (index1 < index2);
    }

    void UpdateRenderables()
    {

        std::vector<Drone::Ref> collection;

        DroneDataPool::DroneFlagIterator<RenderableProp::CachID, Drone> iterator;

        int i = 0;

        // Get a collection of all renderable objects.
        for (auto drone : iterator) {

            if (i >= 512) break;

            collection.push_back(drone);
        }

        // Sort by render pass group
        std::sort(collection.begin(), collection.end(), compareDroneData);

        CachedRenderPass pass = {};

        // For each member in collection, update the data.
        for (int u = 0; u < collection.size(); u++) {

            Drone::Ref & drone = collection[u];
            auto cache         = drone->getCache();

            if (drone->flag != DRONE_FLAG_NEED_RENDER_UPDATE) continue;

            RenderableProp::Ref render = drone->getFirstPropOfType<RenderableProp>();

            unsigned pass_id = render->getRenderPassGroup();

            if (pass.id != pass_id) {

                if (pass.id != -1) render_passes.push_back(pass);

                pass              = {};
                pass.id           = pass_id;
                pass.frame_buffer = 0;
            }

            // Check to see if drone has mesh data -> sort to Objects
            if (cache == MeshProp::DroneDataType) {
                // Process as a renderable object
                setupObject(drone, pass);
                continue;
            }

            // Render Buffer data
            if (cache == RenderLayerProp::DroneDataType) {
                // Input and output textures/buffers.
                setupRenderBuffer(drone, pass);
                continue;
            }

            // Texture data
            if (cache == ImageProp::DroneDataType) {
                // Input and output textures/buffers.

                // Update all texture associated with the drone.
                // setupTexture();
                continue;
            }

            // All others are uniforms.
            setupUniform(drone, pass);
        }

        if (pass.id != -1) render_passes.push_back(pass);
    } // namespace hive

    void RenderBoss::onMessage(StringHash64 message_id, const char * message_data,
                               const unsigned message_length)
    {
    }

    void RenderBoss::setup(){

    };

    // Sort passes based upon textures. If any input texture is dependent on an
    // output texture, then the pass should be ordered before the output object.
    // If there is no dependency, then the order attribute should be used to determine
    // order.
    bool orderRenderPasses(CachedRenderPass & passA, CachedRenderPass & passB)
    {
        char a_ordered_before_b = 0;

        // passA against passB

        for (auto texA : passA.input_textures) {
            for (auto texB : passB.output_textures) {
                if (texA == texB) {
                    a_ordered_before_b = -1;
                    goto outer;
                }
            }
        }


        for (auto texB : passB.input_textures) {
            for (auto texA : passA.output_textures) {
                if (texA == texB) {
                    a_ordered_before_b = 1;
                    goto outer;
                }
            }
        }

    outer:

        if (!a_ordered_before_b) {
            return passA.order < passB.order;
        }

        return (a_ordered_before_b == 1);
    }

    void SortPasses()
    {
        std::sort(render_passes.begin(), render_passes.end(), orderRenderPasses);

        for (auto pass : render_passes) {
        }
    }

    void cullObjects()
    {
        for (auto pass : render_passes) {
            // Get camera information.
            // compare the transformed AABB with bounds to determine if object is renderable.
        }
    }

    void RenderBoss::update(float)
    {

        UpdateRenderables();

        SortPasses();

        cullObjects();

        /**
         *   On message update or intermittent interval, create render settings:
         *
         *      Meshes -> Materials -> Uniforms -> Batches
         *
         *   When ready to render render all batches.
         */

        for (auto pass : render_passes) {

            bool HAVE_PASS_CAMERA = false;

            UniformBinding pass_camera;

            glBindFramebuffer(GL_FRAMEBUFFER, pass.frame_buffer);

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            if (pass.camera_data != nullptr) {

                HIVE_DEBUG_STATIC_WARN("Using raw pointer for global camera data in render pass.");

                HAVE_PASS_CAMERA = true;

                pass_camera.uniform_data = pass.camera_data;
            }

            // Setup camera uniform.
            // Setup any global uniforms.

            for (auto object : pass.objects) {

                glUseProgram(object.program);

                glUniform1i(object.texture.unit, 0);

                glActiveTexture(GL_TEXTURE0 + 0);

                glBindTexture(GL_TEXTURE_2D, object.texture.texture);

                // glActiveTexture(0);
                //
                // glBindTexture(GL_TEXTURE_2D, 0);

                for (auto uniform : object.uniforms) {

                    if (uniform.uniform_type == 11551155 && HAVE_PASS_CAMERA) { // Global camera

                        glUniformMatrix4fv(uniform.uniform_loc, 1, true,
                                           (float *)pass_camera.uniform_data);

                    } else {

                        if (uniform.uniform_data) {

                            float rot = *static_cast<float *>(uniform.uniform_data);

                            glUniform1f(uniform.uniform_loc, rot);
                        }
                    }
                }

                glBindVertexArray(object.vao);

                glDrawElements(GL_TRIANGLES, object.element_count, GL_UNSIGNED_INT, 0);
            }
        }

        glBindVertexArray(0);
    }; // namespace hive

    void RenderBoss::teardown(){

    };

} // namespace hive