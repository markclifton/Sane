#pragma once

#include "sane/layers/base.hpp"
#include "sane/math/vector.hpp"

namespace Sane
{
    class GameWindow : public Layer
    {
    public:
        GameWindow(uint32_t framebufferId);
        virtual void Update() override;

    private:
        Math::Vec2i frameSize;
        uint32_t framebufferId;
    };
}
