#include "sane/systems/ecs/projectile.hpp"

#include "sane/ecs/common.hpp"
#include "sane/logging/log.hpp"

//TODO: Batch projectiles (>10k projectiles would be awesome)

namespace
{
    static const char* vs_projectile = R""(
    #version 330 core
    layout (location = 0) in vec3 vPos;
    uniform mat4 MVP;
    void main()
    {
        gl_Position = MVP * vec4(vPos, 1.0);
    }
    )"";

    static const char* fs_projectile = R""(
    #version 330 core
    out vec4 fragColor;
    void main()
    {
        fragColor = vec4(1.0, 1.0, 1.0, 1.0);
    }
    )"";

    constexpr float vertices[] = { -1.0, 1.0, 0.0, 1.0, 1.0, 0.0, 1.0, -1.0, 0.0, -1.0,-1.0, 0.0 };
}

namespace Sane
{
    namespace ECS
    {
        Projectile::Projectile(entt::registry& registry)
            : SystemBase("ProjectileSystem", registry)
            , Events::Listener("ProjectileSystem")
            , sProg(vs_projectile, fs_projectile)
            , vertices_buffer(GL_ARRAY_BUFFER)
            , vPos(sProg.GetAttribLocation("vPos"), 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0)
        {
        }

        bool Projectile::ProcessEvent(Event& evt)
        {
            if (evt.action == kMouseEvent)
            {
                Input::MouseEvent& me = *(Input::MouseEvent*)evt.data;
                switch (me.action)
                {
                case GLFW_PRESS:
                {
                    heldMouse = true;
                    SpawnProjectile();
                }
                break;
                case GLFW_RELEASE:
                    heldMouse = false;
                    SANE_INFO("Num Projectiles: {}", numProjectiles);
                    break;
                default:
                    return false;
                }
            }

            return false;
        }

        void Projectile::Update(double ts)
        {
            if (heldMouse)
                SpawnProjectile();

            uint32_t numProjectilesAlive = 0;

            auto view = registry_.view<Components::Projectile, Components::Position, Components::Translation>();
            view.each([&](auto entity, Components::Projectile& projectile, Components::Position& position, const Components::Translation& translation) {
                projectile.lifetime += ts;
                if (projectile.lifetime > 2'000)
                {
                    registry_.destroy(entity);
                    return;
                }

                static float speed = .125f;
                position.data.x += speed * translation.x;
                position.data.y -= speed * translation.y;
                position.data.z += speed * translation.z;
                numProjectilesAlive++;
                }
            );

            numProjectiles = numProjectilesAlive;
        }

        void Projectile::RenderScene(glm::mat4 mvp)
        {
            auto view = registry_.view<Components::RenderContext, Components::Camera>();
            view.each([&](const auto entity, const Components::RenderContext& context, const Components::Camera& camera) {
                if (!camera.enabled)
                    return;

                GLint old;
                glGetIntegerv(GL_FRAMEBUFFER_BINDING, &old);

                glBindFramebuffer(GL_FRAMEBUFFER, context.framebuffer);
                glViewport(0, 0, context.width, context.height);

                glClearColor(.05f, .06f, .11f, 1.f);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                sProg.Bind();

                vertices_buffer.Bind();
                vPos.Enable();

                mvp *= camera.lookat;

                auto projView = registry_.view<Components::Projectile, Components::Position>();
                projView.each([&](const auto entity, const Components::Projectile& projectile, const Components::Position& position) {
                    glm::mat4 trans = glm::translate(mvp, { position.data.x, position.data.y, position.data.z });

                    trans[0][0] = 1;
                    trans[0][1] = 0;
                    trans[0][2] = 0;

                    trans[1][0] = 0;
                    trans[1][1] = 1;
                    trans[1][2] = 0;

                    trans[2][0] = 0;
                    trans[2][0] = 0;
                    trans[2][2] = 1;

                    trans = glm::scale(trans, { .125, .125, .125 });

                    glUniformMatrix4fv(sProg.GetUniformLocaition("MVP"), 1, GL_FALSE, (const GLfloat*)&trans[0][0]);
                    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
                    }
                );

                vertices_buffer.Unbind();
                sProg.Unbind();

                glBindFramebuffer(GL_FRAMEBUFFER, old);
                }
            );
        }

        void Projectile::OnAttach()
        {
            sProg.Bind();

            vertices_buffer.Bind();
            vertices_buffer.BufferData(sizeof(float) * 12, &vertices[0], GL_STATIC_DRAW);

            sProg.Unbind();
        }

        void Projectile::SpawnProjectile()
        {
            glm::vec3 velocity;
            glm::vec4 origin;
            auto view = registry_.view<Components::Camera, Components::Position, Components::Player>();
            view.each([&](const auto entity, const Components::Camera& camera, const Components::Position& position, const Components::Player& player) {
                if (!camera.enabled)
                    return;

                velocity = camera.front;
                origin = position.data;
                origin.x += velocity.x;
                origin.y += velocity.y;
                origin.z += velocity.z;

                origin.y += 8;
                }
            );

            const auto projectile = registry_.create();
            registry_.emplace<Components::Projectile>(projectile, 0.f);
            registry_.emplace<Components::Position>(projectile, origin);
            registry_.emplace<Components::Translation>(projectile, velocity.x, velocity.y, velocity.z);
        }
    }
}