#pragma once

#include "sane/math/vector.hpp"
#include "sane/systems/system.hpp"

namespace Sane
{
    class GameWindow : public System
    {
        Math::Vec2i frameSize_;
        uint32_t framebufferId_;

    public:
        GameWindow(uint32_t framebufferId);
        virtual void RenderGui() override;
    };
}
