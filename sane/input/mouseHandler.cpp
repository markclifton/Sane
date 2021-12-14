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
            Instance().SubmitEvent(std::make_unique<Event>(kMouseMoveEvent, &mme, sizeof(mme)));
        }

        void MouseHandler::Process(int button, int action, int mods)
        {
            MouseEvent me{ action, button, mods };
            Instance().SubmitEvent(std::make_unique<Event>(kMouseEvent, &me, sizeof(me)));
        }
    }
}