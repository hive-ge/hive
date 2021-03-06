#pragma once

#include <vector>

#include "include/primitive/core_drone.hpp"
#include "include/primitive/core_math.hpp"
#include "include/primitive/core_prop.hpp"


namespace hive
{
    using namespace hive::math;

    /**::HIVE DRONE_PROP::*/
    struct RenderLayerProp : Prop {
        CONSTRUCT_PROP(RenderLayerProp, renderlayer);

      private:
        /**
         * The render pass group this object belongs to; A render group
         * gathers all relevent objects into a single render pass,
         * which will output data to a renderbuffer.
         */
        unsigned short rp_group = 0;

        /**
         * The render pass order this object belongs to;
         * Layer 0: no output to screen.
         * Layer 1-n: Outputs to screen render buffer.
         */
        unsigned short rb_layer = 0;

        /**
         * This flag determins whether the current object will participate
         * in a render pass.
         */
        bool CAN_RENDER = false;

        /**
         * This flag determins whether the current object will participate
         * in a render pass.
         */
        bool IS_COMPUTE = false;


      public:
        RenderLayerProp() : Prop() {}

        ~RenderLayerProp() {}

        /** Implemented in active GPU library */
        bool SET_RENDER_STATE(bool FLAG);

        bool ALLOW_RENDER() const;

        void setRenderPassGroup(unsigned rpg) { rp_group = 0; };

        const unsigned getRenderPassGroup() const { return rp_group; };

        void setRenderBufferLater(unsigned rbl) { rb_layer = rbl; };

        const unsigned getRenderBufferLater() const { return rb_layer; };
    };

    REGISTER_PROP(RenderLayerProp)
} // namespace hive