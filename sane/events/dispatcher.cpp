#include "sane/events/dispatcher.hpp"

#include "sane/events/queue.hpp"

namespace Sane
{
    namespace Events
    {
        void Dispatcher::SubmitEvent(std::unique_ptr<Event> event)
        {
            Queue::Instance().Submit(std::move(event));
        }
    }
}