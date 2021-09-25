#pragma once

#include "sane/events/events.hpp"

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
            static MouseHandler& Instance()
            {
                static MouseHandler handler;
                return handler;
            }
        public:
            static void Process(double xpos, double ypos)
            {
                MouseMoveEvent mme{ xpos, ypos };
                Event e;
                e.action = kMouseMoveEvent;
                e.detailedAction = 0;
                e.data = &mme;
                e.size = sizeof(mme);
                Instance().SubmitEvent(e);
            }

            static void Process(int button, int action, int mods)
            {
                MouseEvent me{ action, button, mods };
                Event e;
                e.action = kMouseEvent;
                e.detailedAction = action;
                e.data = &me;
                e.size = sizeof(me);
                Instance().SubmitEvent(e);
            }
        };
    }
}