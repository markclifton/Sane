#pragma once

#include <string>

#include "sane/events/event.hpp"

namespace Sane
{
    namespace Events
    {
        class Listener
        {
        public:
            Listener(std::string name);
            ~Listener();

            virtual bool ProcessEvent(Event& evt) = 0;
            inline const std::string Name() { return name_; }
        private:
            const std::string name_;
        };
    }
}