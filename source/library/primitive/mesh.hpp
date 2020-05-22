#pragma once

#include <vector>

#include "primitive/math/math.hpp"

#include "primitive/prop.hpp"

#include "gpu/opengl/gl.hpp"

namespace hive
{
    using namespace hive::math;

    struct NativeMeshData {
        std::vector<vec3> verts;  // local space vertexes
        std::vector<vec3i> faces; // indices
        std::vector<vec2> UVs;    // UV coords
    };

    struct GPUMeshData {
        NativeMeshData * ram_data = nullptr;
        int data_pointer          = 0; // GPU Pointer for data array.
        bool ON_GPU : 1, HAS_UV : 1, HAS_VERT : 1, HAS_INDICES : 1;
    };

    struct MeshData {
        NativeMeshData * native = nullptr;
        GPUMeshData * gpu       = nullptr;
    };

    //::HIVE DRONE_PROP
    struct MeshProp : Prop {

        static MeshProp * construct() { return new MeshProp(); }

        MeshProp() : Prop(MESH_PROP_TYPE, sizeof(MeshProp))
        {

            MeshData * data = new MeshData;

            data->native = new NativeMeshData;
            data->gpu    = new GPUMeshData;

            setData<MeshData>(data);
        }

        ~MeshProp()
        {
            MeshData * data = getData<MeshData>();

            if (data->native) delete data->native;
            if (data->gpu) delete data->gpu;


            delete data;
        }

        void addVertex(float X, float Y, float Z, float U, float V)
        {
            vec3 vec = {X, Y, Z};
            vec2 UV  = {U, V};

            MeshData & data = *getData<MeshData>();

            if (data.native) {
                data.native->verts.push_back(vec);
                data.native->UVs.push_back(UV);
            }
        }

        void uploadVertexData(int location);
        // void removeVertex();
    };

    inline void MeshProp::uploadVertexData(int gpu_pointer) {
        void glBindVertexBuffer(GLuint bindingindex​, GLuint buffer​, GLintptr offset​, GLintptr stride​);
    }
} // namespace hive