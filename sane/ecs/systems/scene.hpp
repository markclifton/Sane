#pragma once

#include <glm/ext.hpp>
#include <glm/glm.hpp>

#include "sane/ecs/common.hpp"
#include "sane/graphics/common.hpp"
#include "sane/model/model.hpp"

namespace Sane
{
    namespace Systems
    {
        class Scene : public System
        {
        public:
            Scene(entt::registry& registry)
                : System(registry)
            {}

            virtual void Update(uint64_t ts) override
            {
                static Model model("models/teapot.obj");

                auto view = registry_.view<Components::RenderContext, Components::Camera, Components::Position, Components::Rotation>();
                view.each([](const auto entity, const Components::RenderContext& context, const Components::Camera& camera, const Components::Position& position, const Components::Rotation& rotation) {
                    if (camera.enabled)
                    {
                        GLint old;
                        glGetIntegerv(GL_FRAMEBUFFER_BINDING, &old);

                        glBindFramebuffer(GL_FRAMEBUFFER, context.framebuffer);
                        glViewport(0, 0, context.width, context.height);

                        glClearColor(.05f, .06f, .11f, 1.f);
                        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                        glm::mat4 rot = glm::rotate(glm::mat4(1.f), rotation.x, glm::vec3(1, 0, 0)) * glm::rotate(glm::mat4(1.f), rotation.y, glm::vec3(0, 1, 0)) * glm::rotate(glm::mat4(1.f), rotation.z, glm::vec3(0, 0, 1));
                        glm::mat4 trans = glm::translate(glm::mat4(1.f), { position.x, position.y, position.z });
                        glm::mat4 p = glm::perspective(45.0f, 16.f / 9.f, 1.0f, 200.0f);
                        glm::mat4 mvp = p * trans * rot;

                        model.DrawImmediate(mvp);

                        glBindFramebuffer(GL_FRAMEBUFFER, old);
                    }
                    });
            }
        };
    }
}
