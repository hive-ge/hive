#pragma once

#include <string>
#include <vector>

#include "include/primitive/core_math.hpp"
#include "include/primitive/core_prop.hpp"
#include "include/primitive/core_string_hash.hpp"
#include "include/primitive/prop_gpu_artifact.hpp"

namespace hive
{

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