#include "sane/events/listener.hpp"

#include "sane/events/queue.hpp"

namespace Sane
{
    namespace Events
    {
        Listener::Listener(std::string name)
            : name_(name)
        {
            Queue::AddListener(this);
        }

        Listener::~Listener()
        {
            Queue::RemoveListener(this);
        }
    }
}