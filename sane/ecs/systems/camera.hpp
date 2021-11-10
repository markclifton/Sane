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
            float pitch{ 0 }, roll{ 0 }, yaw{ -90.f };

            bool enableMouseMovement{ false };
            bool firstMovement{ false };
            Input::MouseMoveEvent lastMousePosition;
            Input::MouseMoveEvent nextMousePosition;

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
                float speed = 1.f;
                if (event.action == kKeyEvent)
                {
                    Input::KeyEvent& keyEvent = *(Input::KeyEvent*)event.data;
                    if (keyEvent.action == GLFW_REPEAT)
                        return false;

                    if (keyEvent.action == GLFW_RELEASE) speed *= -1;

                    switch (keyEvent.key)
                    {
                    case GLFW_KEY_A:
                        x -= speed;
                        break;
                    case GLFW_KEY_D:
                        x += speed;
                        break;
                    case GLFW_KEY_W:
                        z += speed;
                        break;
                    case GLFW_KEY_S:
                        z -= speed;
                        break;
                    case GLFW_KEY_ESCAPE:
                        enableMouseMovement = firstMovement = false;
                        break;
                    default:
                        return false;
                    }
                }
                else if (event.action == kMouseMoveEvent)
                {
                    if (!enableMouseMovement)
                        return false;

                    Input::MouseMoveEvent& mme = *(Input::MouseMoveEvent*)event.data;
                    nextMousePosition = mme;
                    if (!firstMovement)
                    {
                        lastMousePosition = mme;
                        firstMovement = true;
                    }
                }
                else if (event.action == kMouseEvent)
                {
                    Input::MouseEvent& me = *(Input::MouseEvent*)event.data;
                    switch (me.action)
                    {
                    case GLFW_PRESS:
                        enableMouseMovement = true;
                        break;
                    default:
                        return false;
                    }
                }

                return false;
            }

            virtual void Update(uint64_t ts) override
            {

                auto view = registry_.view<Components::Camera, Components::Position, Components::Rotation>();
                view.each([&](const auto entity, Components::Camera& camera, Components::Position& position, Components::Rotation& rotation) {
                    float xoffset = nextMousePosition.xpos - lastMousePosition.xpos;
                    float yoffset = lastMousePosition.ypos - nextMousePosition.ypos;
                    lastMousePosition = nextMousePosition;
                    if (!firstMovement)
                    {
                        xoffset = yoffset = 0.f;
                    }

                    float sensitivity = 0.15f;
                    xoffset *= sensitivity;
                    yoffset *= sensitivity;

                    yaw += xoffset;
                    pitch += yoffset;

                    if (pitch > 89.0f)
                        pitch = 89.0f;
                    if (pitch < -89.0f)
                        pitch = -89.0f;

                    glm::vec3 front;
                    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
                    front.y = sin(glm::radians(pitch));
                    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
                    front = glm::normalize(front);

                    glm::vec3 up = glm::normalize(glm::cross(glm::normalize(glm::cross(front, glm::vec3(0.0f, -1.0f, 0.0f))), front));

                    glm::vec3 ffront = front;
                    ffront.y = 0.f;
                    ffront = glm::normalize(ffront);

                    float cameraSpeed = .025 * ts;
                    glm::vec3 pos(position.x, position.y, position.z);
                    pos += z * cameraSpeed * ffront;
                    pos += x * glm::normalize(glm::cross(ffront, up)) * cameraSpeed;

                    camera.lookat = glm::lookAt(pos, pos + front, up) * glm::scale(glm::mat4(1.f), { 1, -1, 1 });

                    position.x = pos.x;
                    position.y = pos.y;
                    position.z = pos.z;

                    });

                auto view2 = registry_.view<Components::RenderContext, Components::Camera>();
                view2.each([&](const auto entity, const Components::RenderContext& context, const Components::Camera& camera) {
                    if (!camera.enabled)
                        return;

                    static float vertices[]{ -1, 1,-1, 0, 0, -1,-1,-1, 0, 1, 1,-1,-1, 1, 1, 1, 1,-1, 1, 0 };
                    static uint32_t indices[]{ 0, 1, 2, 0, 2, 3 };

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
                );
            }
        };
    }
}
