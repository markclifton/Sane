#include "app.hpp"

#ifdef WIN32
#include <windows.h>
#endif

namespace Sane
{
    App::App()
    {
#if defined(WIN32)
        FreeConsole();
#endif
    }
}