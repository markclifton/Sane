#pragma once

#include "sane/events/event.hpp"

namespace Sane
{
    namespace Events
    {
        class Dispatcher
        {
        public:
            void SubmitEvent(Event& event);
        };
    }
}