#pragma once 

#include "sane/events/dispatcher.hpp"
#include "sane/events/event.hpp"
#include "sane/events/inputs.hpp"
#include "sane/events/listener.hpp"

namespace Sane
{
    const enum EventsType
    {
        kNoEvent = 0,
        kKeyEvent = 1,
        kMouseEvent = 2,
        kMouseMoveEvent = 3,
        kDisplayResizeEvent = 4,
        kDisplaySizeRequestEvent = 5
    };

    struct DisplayResizeEvent
    {
        int width, height;
    };
}