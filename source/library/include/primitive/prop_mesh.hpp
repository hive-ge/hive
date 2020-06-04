#pragma once

#include <vector>

#include "include/primitive/core_math.hpp"
#include "include/primitive/core_prop.hpp"

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

    typedef DataPool::DataField<MeshData> MeshDataField;

    /**::HIVE DRONE_PROP::*/
    struct MeshProp : Prop {

        CONSTRUCT_PROP(MeshProp, mesh);

        MeshDataField::PTR data = nullptr;

        MeshProp() : Prop()
        {
            data         = general_data_pool.allocate<MeshData>();
            data->native = new NativeMeshData;
            data->gpu    = new GPUMeshData;
        }

        ~MeshProp()
        {
            if (data->native) delete data->native;
            if (data->gpu) delete data->gpu;
        }

        void addVertex(float X, float Y, float Z, float U, float V);

        void addTriangle(int a, int b, int c);

        /** Implemented in active GPU library */
        void uploadVertexData(int location);

        /**
         * Create triangles from point data.
         */
        void triangulate();

        bool dataInVRAM();

        unsigned numberOfVertices() const { return data->native->verts.size(); }
    };

    REGISTER_PROP(MeshProp);

    inline std::tuple<vec3d, vec3d> getAABB(MeshProp::Ref & ref)
    {
        vec3d min = {5000000.0, 5000000.0, 5000000.0};
        vec3d max = {0, 0, 0};
        if (ref) {

            HIVE_DEBUG_STATIC_WARN("Should this be locked?");

            MeshDataField::PTR & mesh_data = ref->data;


            for (auto vertex : mesh_data->native->verts) {

                if (vertex.x < min.x) min.x = vertex.x;

                if (vertex.y < min.y) min.y = vertex.y;

                if (vertex.z < min.z) min.z = vertex.z;

                if (vertex.x > max.x) max.x = vertex.x;

                if (vertex.y > max.y) max.y = vertex.y;

                if (vertex.z > max.z) max.z = vertex.z;
            }
        }

        return {min, max};
    };
} // namespace hive