#pragma once

#include <string>
#include <vector>

#include "include/primitive/core_string_hash.hpp"

namespace hive
{

    struct ShaderArtifact {

        enum struct ArtifactType : int {
            DataInput, // Attributes
            DataOutput,
            UniformBlock,
            ShaderBlock,
            Uniform,
            DataBlock,
            TextureInput,
            BufferInput, //
        } type;

        enum struct ElementType : int {
            UNSIGNED = 0,
            INTEGER,
            FLOAT,
            DOUBLE,
            STRUCT,
        } element_type;

        // Number of scalar components withen in an element. Defaults to 1. Up to 16
        int element_size = 1;

        // Number of bytes used for a single element. Defaults to 4.
        int byte_size = 4;

        // Number of elements within an array. Defaults to 1.
        int array_size = 1;

        // Program Pointer location of the element.
        int shader_location = 0;

        StringHash64 name_hash = NullStringHash;

        // Bit field representing the program component that accesses this element.
        int accessed_by = 0;

        // Link to next shader artifact.
        ShaderArtifact * next = nullptr;

      public:
        friend std::ostream & operator<<(std::ostream & stream, const ShaderArtifact & sh)
        {
            stream << "Attribute name: " << sh.name_hash << std::endl;
            stream << "Attribute element type: " << (int)sh.element_type << std::endl;
            stream << "Attribute scalar size: " << sh.element_size << std::endl;
            stream << "Attribute array size: " << sh.array_size << std::endl;
            stream << "Attribute location: " << sh.shader_location << std::endl;
            return stream;
        }
    };
} // namespace hive