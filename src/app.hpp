#pragma once

#include "dlldefines.hpp"

namespace Sane
{
    class SANE_EXPORT App
    {
    public:
        App();
        virtual ~App();
        virtual void Run();
    };

    App* CreateApp();
}