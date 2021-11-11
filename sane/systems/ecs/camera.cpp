#include "sane/systems/ecs/camera.hpp"

namespace
{
    static const char* vs_camera = R""(
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

    static const char* fs_camera = R""(
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
    namespace ECS
    {
        Camera::Camera(entt::registry& registry)
            : SystemBase("CameraSystem", registry)
            , Events::Listener("CameraSystem")
            , sProg(vs_camera, fs_camera)
            , vertices_buffer(GL_ARRAY_BUFFER)
            , normals_buffer(GL_ARRAY_BUFFER)
            , uvs_buffer(GL_ARRAY_BUFFER)
            , indices_buffer(GL_ELEMENT_ARRAY_BUFFER)
            , vPos(sProg.GetAttribLocation("vPos"), 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0)
            , vUV(sProg.GetAttribLocation("vUV"), 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)))
        {
        }

        bool Camera::ProcessEvent(Event& event)
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

        void Camera::Update(double ts)
        {
            auto view = registry_.view<Components::Camera, Components::Position, Components::Rotation>();
            view.each([&](const auto entity, Components::Camera& camera, Components::Position& position, Components::Rotation& rotation) {
                float xoffset = nextMousePosition.xpos - lastMousePosition.xpos;
                float yoffset = lastMousePosition.ypos - nextMousePosition.ypos;
                lastMousePosition = nextMousePosition;
                if (!firstMovement)
                    xoffset = yoffset = 0.f;

                float sensitivity = 0.15f;
                xoffset *= sensitivity;
                yoffset *= sensitivity;

                yaw -= xoffset;
                pitch -= yoffset;

                if (pitch > 89.0f)
                    pitch = 89.0f;
                if (pitch < -89.0f)
                    pitch = -89.0f;

                camera.front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
                camera.front.y = sin(glm::radians(pitch));
                camera.front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
                camera.front = glm::normalize(camera.front);

                glm::vec3 up = glm::normalize(glm::cross(glm::normalize(glm::cross(camera.front, glm::vec3(0.0f, -1.0f, 0.0f))), camera.front));

                glm::vec3 ffront = camera.front;
                ffront.y = 0.f;
                ffront = glm::normalize(ffront);

                float cameraSpeed = .025 * ts;
                glm::vec3 pos(position.data.x, position.data.y, position.data.z);
                pos += z * cameraSpeed * ffront;
                pos += x * glm::normalize(glm::cross(ffront, up)) * cameraSpeed;

                camera.lookat = glm::lookAt(pos, pos + camera.front, up) * glm::scale(glm::mat4(1.f), { 1, -1, 1 });

                position.data.x = pos.x;
                position.data.y = pos.y;
                position.data.z = pos.z;
                }
            );
        }

        void Camera::RenderScene(glm::mat4 mvp)
        {
            auto view = registry_.view<Components::RenderContext, Components::Camera>();
            view.each([&](const auto entity, const Components::RenderContext& context, const Components::Camera& camera) {
                if (!camera.enabled)
                    return;

                mvp = glm::ortho(-1.f, 1.f, 1.f, -1.f, 1.f, 100.f);

                sProg.Bind();
                vertices_buffer.Bind();
                vPos.Enable();
                vUV.Enable();
                indices_buffer.Bind();

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

        void Camera::OnAttach()
        {
            static float vertices[]{ -1, 1,-1, 0, 0, -1,-1,-1, 0, 1, 1,-1,-1, 1, 1, 1, 1,-1, 1, 0 };
            static uint32_t indices[]{ 0, 1, 2, 0, 2, 3 };

            sProg.Bind();

            vertices_buffer.Bind();
            vertices_buffer.BufferData(sizeof(float) * 20, &vertices[0], GL_STATIC_DRAW);

            indices_buffer.Bind();
            indices_buffer.BufferData(sizeof(uint32_t) * 6, &indices[0], GL_STATIC_DRAW);

            indices_buffer.Unbind();
            vertices_buffer.Unbind();
            sProg.Unbind();
        }
    }
}