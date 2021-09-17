#include "logging.hpp"

#include <iostream>

namespace Sane
{
    std::string time_now()
    {
        struct tm time_info;
        time_t time_create = std::time(NULL);
        localtime_s(&time_info, &time_create);
        char timebuf[26];
        asctime_s(timebuf, sizeof(timebuf), &time_info);
        std::string time(timebuf);
        time.pop_back();
        return time;
    }

    Log& Log::Get() {
        static Log log;
        return log;
    }

    void Log::Create(SEVERITY severity)
    {
        this->severity = severity;
    }

    void Log::Destroy()
    {
    }

    void Log::Process(std::function<void(std::stringstream& ss)> fn)
    {
        fn(buffer);
        buffer.clear();
    }

    std::stringstream& Log::Write()
    {
        return Get().buffer;
    }
}