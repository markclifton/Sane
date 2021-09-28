#pragma once

#include "sane/events/dispatcher.hpp"

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
            static KeyHandler& Instance();
        public:
            static void Process(int key, int scancode, int action, int mods);
        };
    }
}