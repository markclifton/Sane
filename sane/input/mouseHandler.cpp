#include "sane/input/mouseHandler.hpp"

#include "sane/events/common.hpp"

namespace Sane
{
    namespace Input
    {
        MouseHandler& MouseHandler::Instance()
        {
            static MouseHandler handler;
            return handler;
        }

        void MouseHandler::Process(double xpos, double ypos)
        {
            MouseMoveEvent mme{ xpos, ypos };
            Event e;
            e.action = kMouseMoveEvent;
            e.detailedAction = 0;
            e.data = &mme;
            e.size = sizeof(mme);
            Instance().SubmitEvent(e);
        }

        void MouseHandler::Process(int button, int action, int mods)
        {
            MouseEvent me{ action, button, mods };
            Event e;
            e.action = kMouseEvent;
            e.detailedAction = action;
            e.data = &me;
            e.size = sizeof(me);
            Instance().SubmitEvent(e);
        }
    }
}