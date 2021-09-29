#pragma once

#include <algorithm>
#include <cmath>
#include <functional>

#include "sane/ecs/components/collidable.hpp"
#include "sane/ecs/components/position.hpp"
#include "sane/ecs/components/translation.hpp"
#include "sane/ecs/systems/system.hpp"
#include "sane/math/vector.hpp"

#include "sane/logging/log.hpp"

namespace Sane
{
    namespace Systems
    {
        class Collision : public System
        {
        public:
            Collision(entt::registry& registry)
                : System(registry)
            {}

            virtual void Update(uint64_t ts) override
            {
                auto bboxTest = [](Components::Position a, Components::Translation t, Components::Position b) -> Components::Translation
                {
                    const float scale = .1f;

                    if (a.y > (scale + b.y)) //above
                    {
                        if (t.y < 0) // going down
                        {
                            float dist = a.y - (scale + b.y);
                            if (dist < std::abs(t.y))
                            {
                                t.y = 0;
                            }
                        }
                    }
                    else if (a.y < b.y) //below
                    {
                        if (t.y > 0) // going up
                        {
                            float dist = b.y - a.y;
                            if (dist < t.y)
                            {
                                t.y = dist;
                            }
                        }
                    }

                    return t;
                };

                auto view = registry_.view<Components::Collidable, Components::Position, Components::Translation>();
                for (auto& [entity, c, p, t] : view.each()) {
                    if (t.y != 0)
                    {
                        for (auto [e2, c2, p2, t2] : view.each()) {
                            if (entity == e2)
                                continue;

                            t = bboxTest(p, t, p2);
                        }
                    }
                    p = { p.x + t.x, p.y + t.y, p.z + t.z };
                }
            }

        private:
        };
    }
}