#pragma once 

#include "sane/events/event.hpp"
#include "sane/events/inputs.hpp"

namespace Sane
{
    const enum EventsType
    {
        kNoEvent = 0,
        kKeyEvent = 1,
        kMouseEvent = 2,
        kMouseMoveEvent = 3
    };
}