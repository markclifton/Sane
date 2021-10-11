#include "sane/ecs/systems/movement.hpp"

#include "sane/logging/log.hpp"

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
                Input::KeyEvent& keyEvent = *(Input::KeyEvent*)event.data;
                if (keyEvent.action == GLFW_REPEAT)
                    return false;

                if (keyEvent.action == GLFW_RELEASE)
                    speed *= -1;

                switch (keyEvent.key)
                {
                case GLFW_KEY_A:
                    x -= speed;
                    return true;
                case GLFW_KEY_D:
                    x += speed;
                    return true;
                case GLFW_KEY_SPACE:
                    if (speed > 0)
                        y = .1f;
                default:
                    return false;
                }
            }
            return false;
        }

        void Movement::Update(uint64_t ts)
        {
            auto view = registry_.view<Components::Player, Components::Translation>();
            view.each([&](const auto entity, const auto& player, auto& t) {
                t.y += y;
                t.x += x;
                y = 0;
                x = 0;
                });
        }
    }
}