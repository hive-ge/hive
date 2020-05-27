

#include "include/hive.h"

namespace hive
{
    void MeshProp::triangulate()
    {
// Assuming all tri
#warning TODO - Implement triangulation routine

        data->native->faces.push_back({0, 1, 2});
        data->native->faces.push_back({2, 3, 0});
    };

    bool MeshProp::dataInVRAM() { return false; };

    void MeshProp::addVertex(float X, float Y, float Z, float U, float V)
    {
        vec3 vec = {X, Y, Z};
        vec2 UV  = {U, V};

        if (data->native) {
            data->native->verts.push_back(vec);
            data->native->UVs.push_back(UV);
        }
    }

    void MeshProp::addTriangle(int a, int b, int c)
    {

        if (data->native) {
            data->native->faces.push_back({a, b, c});
        }
    }
} // namespace hive