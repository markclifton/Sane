#pragma once

#include "sane/events/events.hpp"

namespace Sane
{
    namespace Input
    {
        struct KeyEvent
        {
            int32_t key;
            int32_t scancode;
            int32_t action;
            int32_t mods;
        };


        class KeyHandler : public Events::Dispatcher
        {
        private:
            static KeyHandler& Instance()
            {
                static KeyHandler handler;
                return handler;
            }
        public:
            static void Process(int key, int scancode, int action, int mods)
            {
                KeyEvent ke{ key, scancode, action, mods };
                Event e;
                e.action = kKeyEvent;
                e.detailedAction = action;
                e.data = &ke;
                e.size = sizeof(ke);
                Instance().SubmitEvent(e);
            }
        };
    }
}