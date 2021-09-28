#pragma once

#include "sane/events/dispatcher.hpp"

namespace Sane
{
    namespace Input
    {
        struct MouseEvent
        {
            int action;
            int button;
            int mods;
        };

        struct MouseMoveEvent
        {
            double xpos;
            double ypos;
        };

        class MouseHandler : public Events::Dispatcher
        {
        private:
            static MouseHandler& Instance();
        public:
            static void Process(double xpos, double ypos);
            static void Process(int button, int action, int mods);
        };
    }
}