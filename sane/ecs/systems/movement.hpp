#pragma once

#include "sane/ecs/systems/system.hpp"
#include "sane/ecs/components/player.hpp"
#include "sane/ecs/components/translation.hpp"
#include "sane/events/events.hpp"
#include "sane/events/inputs.hpp"
#include "sane/events/listener.hpp"
#include "sane/logging/log.hpp"

namespace Sane
{
    namespace Systems
    {
        class Movement : public System, public Events::Listener
        {
            float x{ 0 }, y{ 0 }, z{ 0 };
        public:
            Movement(entt::registry& registry)
                : System(registry)
                , Events::Listener("MovementSystem")
            {
            }

            virtual bool ProcessEvent(Event& event) override
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

            virtual void Update(uint64_t ts) override
            {
                auto view = registry_.view<Components::Player, Components::Translation>();
                view.each([&](const auto entity, const auto& player, auto& t) {
                    t.y += y;
                    t.x += x;
                    y = 0;
                    x = 0;
                    });
            }

        };
    }
}