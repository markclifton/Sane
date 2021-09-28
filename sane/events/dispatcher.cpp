#include "sane/events/dispatcher.hpp"

#include "sane/events/queue.hpp"

namespace Sane
{
    namespace Events
    {
        void Dispatcher::SubmitEvent(Event& event)
        {
            Queue::Instance().Submit(event);
        }
    }
}