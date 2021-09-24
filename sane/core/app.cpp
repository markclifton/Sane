#include "app.hpp"

#ifdef WIN32
#include <windows.h>
#endif

namespace Sane
{
    App::App()
        : evt_queue(EventQueue::Instance())
    {
#if defined(WIN32)
        FreeConsole();
#endif
        layers.PushLayer(&evt_queue);
    }
}