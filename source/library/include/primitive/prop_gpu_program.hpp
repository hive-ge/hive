#pragma once

#include <string>
#include <vector>

#include "include/primitive/core_math.hpp"
#include "include/primitive/core_prop.hpp"
#include "include/primitive/core_string_hash.hpp"

namespace hive
{
    using namespace hive::math;


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
    };

    struct ShaderProgramPropData {
        std::vector<ShaderArtifact> artifacts;
        int program = 0;
    };

    //::HIVE DRONE_PROP
    struct ShaderProgramProp : Prop {

      private:
        ShaderProgramPropData * data = nullptr;

      public:
        inline static ShaderProgramProp * construct() { return new ShaderProgramProp(); }

        ShaderProgramProp();

        ~ShaderProgramProp();

        /**
         *  Load program artifacts from a formatted string. String format dependent upon
         * implementation.
         */
        bool fromString(std::string file_data);

        /**
         * Load program artifacts from binary data. Binary data format dependent upon
         * implementation.
         */
        bool fromBinary(const char * buffer, const unsigned buffer_size);

        /**
         * True if the program is compiled and ready for use for GPU render passes.
         */
        bool IS_AVAILABLE_ON_GPU();


        /**
         * Retrieves a map of all inputs that are available in the program.
         */
        const std::vector<ShaderArtifact> & getInputMap();

        /**
         * Retrieves a map of all ouputs that are available in the program.
         */
        const std::vector<ShaderArtifact> & getOutputMap();
    };
} // namespace hive