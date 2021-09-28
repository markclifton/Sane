#pragma once

#include "sane/layers/base.hpp"

namespace Sane
{
    class FpsCounter : public Layer
    {
    public:
        FpsCounter();
        virtual void Update() override;
    };
}