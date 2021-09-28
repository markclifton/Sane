#pragma once

#include "sane/layers/layer.hpp"
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
