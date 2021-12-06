#include "sane/systems/ecs/scene.hpp"

#include "sane/ecs/common.hpp"
#include "sane/model/model.hpp"

namespace Sane
{
    namespace ECS
    {
        Scene::Scene(entt::registry& registry)
            : SystemBase("SceneSystem", registry)
        {}

        void Scene::RenderScene(glm::mat4 mvp)
        {
            static Model model("models/cube.obj");

            auto view = registry_.view<Components::RenderContext, Components::Camera, Components::Position, Components::Rotation>();
            view.each([&](const auto entity, const Components::RenderContext& context, const Components::Camera& camera, const Components::Position& position, const Components::Rotation& rotation) {
                if (!camera.enabled)
                    return;

                GLint old;
                glGetIntegerv(GL_FRAMEBUFFER_BINDING, &old);

                glBindFramebuffer(GL_FRAMEBUFFER, context.framebuffer);
                glViewport(0, 0, context.width, context.height);

                model.DrawImmediate(mvp * camera.lookat);

                glBindFramebuffer(GL_FRAMEBUFFER, old);
                }
            );
        }
    }
}