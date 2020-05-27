#pragma once

#include <string>
#include <vector>

#include "include/primitive/core_string_hash.hpp"

namespace hive
{

    struct ShaderArtifact {

        enum struct ArtifactType : int {
            VertexData, // Attributes
            DataOutput,
            UniformBlock,
            ShaderBlock,
            Uniform,
            DataBlock,
            TextureInput,
            BufferInput, //
        } type;

        enum struct ElementPrimitiveType : int {
            INTEGER,
            FLOAT,
            DOUBLE,
            UNSIGNED_INTEGER,
            STRUCT,
        } element_type;

        // Number of numerical components withen in an element. Defaults to 1. Up to 16 for matrices
        int unit_size = 1;

        // Number of bytes used for a single element. Defaults to 4.
        int byte_size = 4;

        // Number of elements within an array. Defaults to 1.
        int array_size = 1;

        // Program Pointer location of the element.
        int shader_location = -1;

        StringHash64 name_hash = NullStringHash;

      public:
        friend std::ostream & operator<<(std::ostream & stream, const ShaderArtifact & sh)
        {
            stream << "Attribute name: " << sh.name_hash << std::endl;
            stream << "Attribute type: " << (int)sh.type << std::endl;
            stream << "Attribute element type: " << (int)sh.element_type << std::endl;
            stream << "Attribute unit size: " << sh.unit_size << std::endl;
            stream << "Attribute byte size: " << sh.byte_size << std::endl;
            stream << "Attribute array size: " << sh.array_size << std::endl;
            stream << "Attribute location: " << sh.shader_location << std::endl;
            return stream;
        }
    };
} // namespace hive