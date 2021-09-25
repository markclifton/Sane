#pragma once 

#include "sane/events/event.hpp"
#include "sane/events/inputs.hpp"

namespace Sane
{
    enum EventsType
    {
        kKeyEvent = 1,
        kMouseEvent,
        kMouseMoveEvent
    };
}