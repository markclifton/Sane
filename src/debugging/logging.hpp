#pragma once

#include <sstream>
#include <functional>

#include <iostream>
using namespace std;

#include "dlldefines.hpp"

#define ENABLE_LOGS(SEVERITY) Sane::Log::Get().Create(SEVERITY);
#define DISABLE_LOGS() Sane::Log::Get().Destroy();
#define PROCESS_LOGS(fn) Sane::Log::Get().Process(fn);

#define SANE_DEBUG(s) if(Sane::Log::Get().severity <= Sane::SEVERITY::DEBUG) Sane::Log::Write() << "[DEBUG] " << Sane::time_now() << " :: " << s << "\n";
#define SANE_INFO(s)  if(Sane::Log::Get().severity <= Sane::SEVERITY::INFO)  Sane::Log::Write() << "[INFO]  " << Sane::time_now() << " :: " << s << "\n";
#define SANE_WARN(s)  if(Sane::Log::Get().severity <= Sane::SEVERITY::WARN)  Sane::Log::Write() << "[WARN]  " << Sane::time_now() << " :: " << s << "\n";
#define SANE_FATAL(s) if(Sane::Log::Get().severity <= Sane::SEVERITY::FATAL) Sane::Log::Write() << "[FATAL] " << Sane::time_now() << " :: " << s << "\n";

namespace Sane
{
    std::string SANE_EXPORT time_now();

    enum SEVERITY {
        DEBUG = 0,
        INFO = 1,
        WARN = 2,
        FATAL = 3
    };

    class SANE_EXPORT Log {
    public:
        static Log& Get();
        void Create(SEVERITY minLevel = SEVERITY::DEBUG);
        void Destroy();

        void Process(std::function<void(std::stringstream& ss)> fn);
        static std::stringstream& Write();

    private:
        std::stringstream buffer;

    public:
        SEVERITY severity;
    };
}