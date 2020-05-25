
/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    DO NOT MODIFY - This file is automatically created by the build:props script.

    COPYRIGHT 2020 Anthony C Weathersby
 !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/

#pragma once
#include <thread>
#include <vector>
#include <hive.h>
#include <emscripten/bind.h>


namespace hive{
    namespace js{

        using namespace hive::all;

        EMSCRIPTEN_BINDINGS(hive_property_bindings) {
            emscripten::class_<Prop>("Prop")
	.constructor(&Prop::construct, emscripten::allow_raw_pointers())
	.function("connect", &Prop::connect, emscripten::allow_raw_pointers())
	.function("disconnect", &Prop::disconnect);

emscripten::class_<Drone>("Drone")
	.constructor(&Drone::construct, emscripten::allow_raw_pointers())
	.function("signalUpdate", &Drone::signalUpdate)
	.function("connect", &Drone::connect, emscripten::allow_raw_pointers())
	.function("disconnect", &Drone::disconnect, emscripten::allow_raw_pointers());

emscripten::class_<ShaderProgramProp, emscripten::base<Prop>>("ShaderProgramProp")
	.constructor(&ShaderProgramProp::construct, emscripten::allow_raw_pointers())
	.function("fromString", &ShaderProgramProp::fromString)
	.function("fromBinary", &ShaderProgramProp::fromBinary, emscripten::allow_raw_pointers())
	.function("IS_AVAILABLE_ON_GPU", &ShaderProgramProp::IS_AVAILABLE_ON_GPU)
	.function("getInputMap", &ShaderProgramProp::getInputMap, emscripten::allow_raw_pointers())
	.function("getOutputMap", &ShaderProgramProp::getOutputMap, emscripten::allow_raw_pointers());

emscripten::class_<ImageProp, emscripten::base<Prop>>("ImageProp")
	.constructor(&ImageProp::construct, emscripten::allow_raw_pointers())
	.function("loadFromFile", &ImageProp::loadFromFile)
	.function("loadFromRawBinaryData", &ImageProp::loadFromRawBinaryData, emscripten::allow_raw_pointers())
	.function("uploadToVRAM", &ImageProp::uploadToVRAM)
	.function("downloadFromVRAM", &ImageProp::downloadFromVRAM)
	.function("clearFromRAM", &ImageProp::clearFromRAM)
	.function("getRawData", &ImageProp::getRawData, emscripten::allow_raw_pointers())
	.function("IS_IN_VRAM", &ImageProp::IS_IN_VRAM)
	.function("IS_IN_RAM", &ImageProp::IS_IN_RAM)
	.function("setRenderTarget", &ImageProp::setRenderTarget)
	.function("clearRenderTarget", &ImageProp::clearRenderTarget);

emscripten::class_<DoubleMat44Prop, emscripten::base<Prop>>("DoubleMat44Prop")
	.constructor(&DoubleMat44Prop::construct, emscripten::allow_raw_pointers());

emscripten::class_<MeshProp, emscripten::base<Prop>>("MeshProp")
	.constructor(&MeshProp::construct, emscripten::allow_raw_pointers())
	.function("addVertex", &MeshProp::addVertex)
	.function("uploadVertexData", &MeshProp::uploadVertexData);

emscripten::class_<RenderableProp, emscripten::base<Prop>>("RenderableProp")
	.constructor(&RenderableProp::construct, emscripten::allow_raw_pointers())
	.function("SET_RENDER_STATE", &RenderableProp::SET_RENDER_STATE)
	.function("ALLOW_RENDER", &RenderableProp::ALLOW_RENDER);

emscripten::class_<FloatVec3Prop, emscripten::base<Prop>>("FloatVec3Prop")
	.constructor(&FloatVec3Prop::construct, emscripten::allow_raw_pointers());

emscripten::class_<DoubleVec3Prop, emscripten::base<Prop>>("DoubleVec3Prop")
	.constructor(&DoubleVec3Prop::construct, emscripten::allow_raw_pointers()); 
        }
    }
}