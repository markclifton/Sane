#pragma once

#include "sane/layers/layer.hpp"

namespace Sane
{
    class FpsCounter : public Layer
    {
    public:
        virtual void Process() override;
    };
}