#pragma once

#include <glm/ext.hpp>
#include <glm/glm.hpp>

#include "sane/ecs/common.hpp"
#include "sane/ecs/systems/system.hpp"
#include "sane/events/common.hpp"
#include "sane/graphics/common.hpp"

namespace
{
    static const char* vs_modern_tex = R""(
    #version 330
    layout(location = 0) in vec3 vPos;
    layout(location = 1) in vec2 vUV;
    uniform mat4 MVP;
    out vec2 f_uv;
    void main() {
      gl_Position = MVP * vec4(vPos, 1.0);
      f_uv = vUV;
    }
    )"";

    static const char* fs_modern_tex = R""(
    #version 330
    out vec4 outColor;
    in vec2 f_uv;
    uniform sampler2D tex;
    void main() {
      outColor = texture(tex, f_uv);
    }
    )"";
}

namespace Sane
{
    namespace Systems
    {
        class Camera : public System, public Events::Listener
        {
            bool buffered_{ false };

            ShaderProgram sProg;
            Buffer vertices_buffer;
            Buffer normals_buffer;
            Buffer uvs_buffer;
            Buffer indices_buffer;
            VertexAttrib vPos;
            VertexAttrib vUV;
        public:
            float x{ 0 }, y{ 0 }, z{ 0 };

            Camera(entt::registry& registry)
                : System(registry)
                , Events::Listener("CameraSystem")
                , sProg(vs_modern_tex, fs_modern_tex)
                , vertices_buffer(GL_ARRAY_BUFFER)
                , normals_buffer(GL_ARRAY_BUFFER)
                , uvs_buffer(GL_ARRAY_BUFFER)
                , indices_buffer(GL_ELEMENT_ARRAY_BUFFER)
                , vPos(sProg.GetAttribLocation("vPos"), 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0)
                , vUV(sProg.GetAttribLocation("vUV"), 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)))
            {}

            virtual bool ProcessEvent(Event& event) override
            {
                float speed = .025f;
                if (event.action == kKeyEvent)
                {
                    Input::KeyEvent& keyEvent = *(Input::KeyEvent*)event.data;
                    if (keyEvent.action == GLFW_REPEAT)
                        return false;

                    if (keyEvent.action == GLFW_RELEASE) speed *= -1;

                    switch (keyEvent.key)
                    {
                    case GLFW_KEY_A:
                        x += speed;
                        break;
                    case GLFW_KEY_D:
                        x -= speed;
                        break;
                    case GLFW_KEY_W:
                        y -= speed;
                        break;
                    case GLFW_KEY_S:
                        y += speed;
                        break;
                    default:
                        return false;
                    }
                }
                return false;
            }

            virtual void Update(uint64_t ts) override
            {

                {
                    auto view = registry_.view<Components::Camera, Components::Position, Components::Rotation>();
                    view.each([&](const auto entity, Components::Camera& camera, Components::Position& position, Components::Rotation& rotation) {
                        position.x += x;
                        position.y += y;
                        position.z += z;
                        });

                    //x = y = z = 0.f;
                }

                auto view = registry_.view<Components::RenderContext, Components::Camera>();
                view.each([&](const auto entity, const Components::RenderContext& context, const Components::Camera& camera) {
                    if (camera.enabled)
                    {

                        static float vertices[]
                        {
                            -1, 1,-1, 0, 0,
                            -1,-1,-1, 0, 1,
                             1,-1,-1, 1, 1,
                             1, 1,-1, 1, 0
                        };

                        static uint32_t indices[]
                        {
                            0, 1, 2,
                            0, 2, 3
                        };

                        glm::mat4 m = glm::mat4(1.f);
                        glm::mat4 p = glm::ortho(-1.f, 1.f, 1.f, -1.f, 1.f, 100.f);
                        glm::mat4 mvp = p * m;

                        sProg.Bind();

                        vertices_buffer.Bind();
                        if (!buffered_)
                            vertices_buffer.BufferData(sizeof(float) * 20, &vertices[0], GL_STATIC_DRAW);

                        vPos.Enable();
                        vUV.Enable();

                        indices_buffer.Bind();
                        if (!buffered_)
                            indices_buffer.BufferData(sizeof(uint32_t) * 6, &indices[0], GL_STATIC_DRAW);

                        glUniformMatrix4fv(sProg.GetUniformLocaition("MVP"), 1, GL_FALSE, (const GLfloat*)&mvp[0][0]);

                        glBindTexture(GL_TEXTURE_2D, context.attachments[0]);
                        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);
                        glBindTexture(GL_TEXTURE_2D, 0);

                        indices_buffer.Unbind();

                        vertices_buffer.Unbind();

                        sProg.Unbind();
                    }
                    }
                );
            }
        };
    }
}
