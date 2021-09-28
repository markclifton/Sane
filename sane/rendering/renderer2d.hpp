#pragma once

#include "sane/graphics/framebuffer.hpp"
#include "sane/layers/base.hpp"

namespace Sane
{
    namespace Rendering
    {
        class Renderer2D : public Layer
        {
        public:
            Renderer2D()
            : Layer("Renderer2D")
            , framebuffer_(1280, 720)
            {
            }

            virtual void Update() override
            {
            }

        private:
            Framebuffer framebuffer_;
        };
    }
}