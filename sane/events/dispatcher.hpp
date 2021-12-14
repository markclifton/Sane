#pragma once

#include <memory>

#include "sane/events/event.hpp"

namespace Sane
{
    namespace Events
    {
        class Dispatcher
        {
        public:
            void SubmitEvent(std::unique_ptr<Event> event);
        };
    }
}