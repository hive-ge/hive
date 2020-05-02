#pragma once

#include <string>
#include <vector>

#include "primitive/math/math.hpp"

#include "primitive/prop.hpp"

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

        int element_size    = 0;
        int byte_size       = 0;
        int array_size      = 0;
        int shader_location = 0;
        int accessed_by     = 0;
    };

    enum ShaderProgramIndices : char {
        GEOMETRY_SHADER,
        FRAGMENT_SHADER,
        VERTEX_SHADER,
        COMPUTE_SHADER,
        TESSELATION_EVAL_SHADER,
        TESSELATION_CTRL_SHADER
    };

    struct ShaderProgramPropData {
        std::vector<ShaderArtifact> artifacts;
        int program      = 0;
        int programs[10] = {};
    };

    //::HIVE DRONE_PROP
    struct ShaderProgramProp : Prop {

        static ShaderProgramProp * construct() { return new ShaderProgramProp(); }

        ShaderProgramProp() : Prop(SHADER_PROGRAM_PROP_TYPE, sizeof(ShaderProgramProp))
        {
            // TODO use a hex string and LU table to store duplicate programs.
            setData<ShaderProgramPropData>(new ShaderProgramPropData);
        }

        ~ShaderProgramProp()
        {
            ShaderProgramPropData * data = getData<ShaderProgramPropData>();
            delete data;
        }


        void configureProgramInterfaces();

        void setupStaticInputs(Drone * drone) {}


        bool addVertexShader(std::string & vertex_shader_string);

        bool addFragShader(std::string & frag_shader_string);

        bool addComputeShader(std::string & comput_shader_string);

        bool addGeometryShader(std::string & geometry_shader_string);

        bool addTesselationEvalShader(std::string & tesselation_eval_shader);

        bool addTesselationControlShader(std::string & tesselation_ctrl_shader);

        bool compileShader();

        /**
         * Parse program information and build connections for input, output, textures.
         */
        void configureProgramInputs();

        // bool addVertexShader(std::string & vertex_shader_string) {}
        // bool addFragShader(std::string & vertex_shader_string) {}
        // bool addComputeShader(std::string & vertex_shader_string) {}
        // bool addGeometryShader(std::string & vertex_shader_string) {}
        // bool addTexelShader(std::string & vertex_shader_string) {}
        // void removeVertex();
      private:
        bool deleteShaders();
    };
} // namespace hive