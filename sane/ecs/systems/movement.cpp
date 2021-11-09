#include "sane/ecs/systems/movement.hpp"

#include "sane/logging/log.hpp"
#include <GLFW/glfw3.h>

namespace Sane
{
    namespace Systems
    {
        Movement::Movement(entt::registry& registry)
            : System(registry)
            , Events::Listener("MovementSystem")
        {
        }

        bool Movement::ProcessEvent(Event& event)
        {
            float speed = .025f;
            if (event.action == kKeyEvent)
            {
                (void)z;
                Input::KeyEvent& keyEvent = *(Input::KeyEvent*)event.data;
                if (keyEvent.action == GLFW_REPEAT)
                    return false;

                if (keyEvent.action == GLFW_RELEASE)
                    speed *= -1;

                switch (keyEvent.key)
                {
                case GLFW_KEY_A:
                    x -= speed;
                    return false;
                case GLFW_KEY_D:
                    x += speed;
                    return false;
                case GLFW_KEY_SPACE:
                    if (speed > 0)
                        y = .1f;
                    return false;
                default:
                    return false;
                }
            }
            return false;
        }

        void Movement::Update(uint64_t ts)
        {
        }
    }
}