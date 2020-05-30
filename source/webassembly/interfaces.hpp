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
            emscripten::class_<ImageProp,emscripten::base<hive::Prop>>("ImageProp").constructor(&ImageProp::construct,emscripten::allow_raw_pointers()).function("loadFromFile", &ImageProp::loadFromFile).function("loadFromRawBinaryData", &ImageProp::loadFromRawBinaryData,emscripten::allow_raw_pointers()).function("setPixel", &ImageProp::setPixel).function("uploadToVRAM", &ImageProp::uploadToVRAM).function("downloadFromVRAM", &ImageProp::downloadFromVRAM).function("clearFromRAM", &ImageProp::clearFromRAM).function("getRawData", &ImageProp::getRawData).function("IS_IN_VRAM", &ImageProp::IS_IN_VRAM).function("IS_IN_RAM", &ImageProp::IS_IN_RAM).function("setRenderTarget", &ImageProp::setRenderTarget).function("clearRenderTarget", &ImageProp::clearRenderTarget).function("setSize", &ImageProp::setSize);

emscripten::class_<MeshProp,emscripten::base<hive::Prop>>("MeshProp").constructor(&MeshProp::construct,emscripten::allow_raw_pointers()).function("addVertex", &MeshProp::addVertex).function("addTriangle", &MeshProp::addTriangle).function("uploadVertexData", &MeshProp::uploadVertexData).function("triangulate", &MeshProp::triangulate).function("dataInVRAM", &MeshProp::dataInVRAM).function("numberOfVertices", &MeshProp::numberOfVertices);

emscripten::class_<Drone>("Drone").constructor(&Drone::construct,emscripten::allow_raw_pointers()).function("connect", &Drone::connect).function("disconnect", &Drone::disconnect);

emscripten::class_<Prop>("Prop").constructor(&Prop::construct,emscripten::allow_raw_pointers()).function("connect", &Prop::connect,emscripten::allow_raw_pointers()).function("disconnect", &Prop::disconnect,emscripten::allow_raw_pointers()).function("getTagHash", &Prop::getTagHash).function("getTag", &Prop::getTag).function("setTag", &Prop::setTag);

emscripten::class_<ShaderProgramProp,emscripten::base<hive::Prop>>("ShaderProgramProp").constructor(&ShaderProgramProp::construct,emscripten::allow_raw_pointers()).function("fromString", &ShaderProgramProp::fromString).function("fromBinary", &ShaderProgramProp::fromBinary,emscripten::allow_raw_pointers()).function("IS_AVAILABLE_ON_GPU", &ShaderProgramProp::IS_AVAILABLE_ON_GPU).function("getInputMap", &ShaderProgramProp::getInputMap).function("getOutputMap", &ShaderProgramProp::getOutputMap);

emscripten::class_<DoubleMat44Prop,emscripten::base<hive::Prop>>("DoubleMat44Prop").constructor(&DoubleMat44Prop::construct,emscripten::allow_raw_pointers());

emscripten::class_<FloatVec3Prop,emscripten::base<hive::Prop>>("FloatVec3Prop").constructor(&FloatVec3Prop::construct,emscripten::allow_raw_pointers());

emscripten::class_<DoubleVec3Prop,emscripten::base<hive::Prop>>("DoubleVec3Prop").constructor(&DoubleVec3Prop::construct,emscripten::allow_raw_pointers());

emscripten::class_<FloatProp,emscripten::base<hive::Prop>>("FloatProp").constructor(&FloatProp::construct,emscripten::allow_raw_pointers()).function("set", &FloatProp::set);

emscripten::class_<RenderableProp,emscripten::base<hive::Prop>>("RenderableProp").constructor(&RenderableProp::construct,emscripten::allow_raw_pointers()).function("SET_RENDER_STATE", &RenderableProp::SET_RENDER_STATE).function("ALLOW_RENDER", &RenderableProp::ALLOW_RENDER);
        }
    }
}