#pragma once

#include <cstdint>

namespace Sane
{
    struct Event
    {
        uint32_t action{ 0 };
        uint32_t detailedAction{ 0 };
        void* data{ nullptr };
        uint32_t size{ 0 };

        Event();
        Event(uint32_t action, void* srcData, uint32_t size);
        Event(uint32_t action, uint32_t detailedAction, void* srcData, uint32_t size);
    };
}