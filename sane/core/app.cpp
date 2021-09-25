#include "app.hpp"

#ifdef WIN32
#include <windows.h>
#endif

namespace Sane
{
    App::App()
        : evt_queue_(Events::Queue::Instance())
    {
#if defined(WIN32)
        FreeConsole();
#endif
        layers_.PushLayer(&evt_queue_);
    }
}