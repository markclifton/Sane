#pragma once

#include <string>

namespace Sane
{
    struct LoadEvent
    {
        LoadEvent(std::string path)
        {
            memcpy(filepath, path.c_str(), path.size());
            size = path.size();
        }

        char filepath[1024];
        uint32_t size{ 0 };
    };
}