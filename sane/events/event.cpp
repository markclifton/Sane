#include "sane/events/event.hpp"

#include <cstdlib>
#include <cstring>

namespace Sane
{
    Event::Event() {}

    Event::Event(uint32_t action, void* srcData, uint32_t size)
        : action(action), size(size)
    {
        data = malloc(size);
        memcpy(data, srcData, size);
    }

    Event::Event(uint32_t action, uint32_t detailedAction, void* srcData, uint32_t size)
        : action(action), detailedAction(detailedAction), size(size)
    {
        data = malloc(size);
        memcpy(data, srcData, size);
    }
}