
/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    DO NOT MODIFY - This file is automatically created by the build:props script.

    COPYRIGHT 2020 Anthony C Weathersby
 !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/

#pragma once
#include <thread>
#include <vector>
#include <hive.h>

using namespace hive::all;

namespace hive{
    namespace js{
        EMSCRIPTEN_BINDINGS(hive_property_bindings) {
            class_<Prop>("Prop})
	.function(getData, &Prop::getData)
	.function(setData, &Prop::setData)
	.function(setID, &Prop::setID)
	.function(getID, &Prop::getID)
	.function(connect, &Prop::connect)
	.function(disconnect, &Prop::disconnect);
class_<SpriteProp>("SpriteProp});
class_<Drone>("Drone})
	.function(setBoss, &Drone::setBoss)
	.function(getBoss, &Drone::getBoss)
	.function(signalUpdate, &Drone::signalUpdate)
	.function(connect, &Drone::connect)
	.function(disconnect, &Drone::disconnect);
class_<ShaderProgramProp>("ShaderProgramProp})
	.function(configureProgramInterfaces, &ShaderProgramProp::configureProgramInterfaces)
	.function(setupStaticInputs, &ShaderProgramProp::setupStaticInputs)
	.function(addVertexShader, &ShaderProgramProp::addVertexShader)
	.function(addFragShader, &ShaderProgramProp::addFragShader)
	.function(addComputeShader, &ShaderProgramProp::addComputeShader)
	.function(addGeometryShader, &ShaderProgramProp::addGeometryShader)
	.function(addTesselationEvalShader, &ShaderProgramProp::addTesselationEvalShader)
	.function(addTesselationControlShader, &ShaderProgramProp::addTesselationControlShader)
	.function(compileShader, &ShaderProgramProp::compileShader)
	.function(configureProgramInputs, &ShaderProgramProp::configureProgramInputs)
	.function(deleteShaders, &ShaderProgramProp::deleteShaders);
class_<ImageProp>("ImageProp})
	.function(loadFromFile, &ImageProp::loadFromFile)
	.function(loadFromRawBinaryData, &ImageProp::loadFromRawBinaryData)
	.function(uploadToVRAM, &ImageProp::uploadToVRAM)
	.function(downloadFromVRAM, &ImageProp::downloadFromVRAM)
	.function(clearFromRAM, &ImageProp::clearFromRAM)
	.function(getRawData, &ImageProp::getRawData)
	.function(IS_IN_VRAM, &ImageProp::IS_IN_VRAM)
	.function(IS_IN_RAM, &ImageProp::IS_IN_RAM)
	.function(setRenderTarget, &ImageProp::setRenderTarget)
	.function(clearRenderTarget, &ImageProp::clearRenderTarget)
	.function(getTextureHandle, &ImageProp::getTextureHandle);
class_<FloatMat22Prop>("FloatMat22Prop})
	.function(placeInGPU, &FloatMat22Prop::placeInGPU);
class_<DoubleMat22Prop>("DoubleMat22Prop});
class_<FloatMat33Prop>("FloatMat33Prop});
class_<FloatMat44Prop>("FloatMat44Prop});
class_<DoubleMat33Prop>("DoubleMat33Prop});
class_<DoubleMat44Prop>("DoubleMat44Prop});
class_<MeshProp>("MeshProp})
	.function(addVertex, &MeshProp::addVertex)
	.function(uploadVertexData, &MeshProp::uploadVertexData);
class_<FloatVec3Prop>("FloatVec3Prop});
class_<DoubleVec3Prop>("DoubleVec3Prop}); 
        }
    }
}