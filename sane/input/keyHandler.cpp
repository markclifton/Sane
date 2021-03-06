#include "sane/input/keyHandler.hpp"

#include "sane/events/common.hpp"

namespace Sane
{
    namespace Input
    {
        KeyHandler& KeyHandler::Instance()
        {
            static KeyHandler handler;
            return handler;
        }

        void KeyHandler::Process(int key, int scancode, int action, int mods)
        {
            KeyEvent ke{ key, scancode, action, mods };
            Instance().SubmitEvent(std::make_unique<Event>(kKeyEvent, action, &ke, sizeof(ke)));
        }
    }
}