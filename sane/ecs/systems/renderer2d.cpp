#include "sane/ecs/systems/renderer2d.hpp"

#include <entt/entt.hpp>

#include "sane/ecs/components/position.hpp"
#include "sane/ecs/components/scale.hpp"
#include "sane/ecs/components/translation.hpp"
#include "sane/ecs/systems/system.hpp"

#include "sane/graphics/buffer.hpp"
#include "sane/graphics/shaderprogram.hpp"

namespace
{
    float vertices[] = {
         0.5f,  0.5f, -1.0f,   1.0f, 0.0f, 0.0f,
         0.5f, -0.5f, -1.0f,   0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, -1.0f,   0.0f, 0.0f, 1.0f,
        -0.5f,  0.5f, -1.0f,   1.0f, 1.0f, 0.0f
    };
    unsigned int indices[] = {
        0, 1, 3,
        1, 2, 3
    };

    static const char* vs_modern = R""(
    #version 330
    layout(location = 0) in vec3 vPos;
    layout(location = 1) in vec3 vCol;
    out vec3 color;
    uniform mat4 MVP;
    void main() {
      gl_Position = MVP * vec4(vPos, 1.0);
      color = vCol;
    }
)"";

    static const char* fs_modern = R""(
    #version 330
    out vec4 outColor;
    in vec3 color;
    void main() {
      outColor = vec4(color, 1);
    }
)"";
}

namespace Sane
{
    namespace Systems
    {
        Renderer2d::Renderer2d(entt::registry& registry)
            : System(registry)
            , sProg_(vs_modern, fs_modern)
            , vbo_(GL_ARRAY_BUFFER)
            , ibo_(GL_ELEMENT_ARRAY_BUFFER)
            , vPos(sProg_.GetAttribLocation("vPos"), 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0)
            , vCol(sProg_.GetAttribLocation("vCol"), 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)))
        {
            sProg_.Bind();
            vbo_.Bind();
            vbo_.BufferData(sizeof(vertices), vertices, GL_STATIC_DRAW);

            ibo_.Bind();
            ibo_.BufferData(sizeof(indices), indices, GL_STATIC_DRAW);

            ibo_.Unbind();
            vbo_.Unbind();

            mvp_location = sProg_.GetUniformLocaition("MVP");
            sProg_.Unbind();
        }

        void Renderer2d::Update(uint64_t ts)
        {
            auto view = registry_.view<Components::Position, Components::Scale, Components::Translation>();
            for (auto [entity, pos, scale, trans] : view.each()) {
                glm::mat4 m = glm::translate(glm::mat4(1.f), { pos.x, pos.y, pos.z }) * glm::translate(glm::mat4(1.f), { trans.x, trans.y, trans.z }) * glm::scale(glm::mat4(1.f), { scale.x, scale.y, 1 });
                glm::mat4 p = glm::perspective(45.0f, 16.f / 9.f, 1.0f, 200.0f);
                glm::mat4 mvp = p * m;

                sProg_.Bind();
                vbo_.Bind();
                vPos.Enable();
                vCol.Enable();

                ibo_.Bind();
                glUniformMatrix4fv(mvp_location, 1, GL_FALSE, (const GLfloat*)&mvp[0][0]);
                glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);
                ibo_.Unbind();

                vbo_.Unbind();
                sProg_.Unbind();
            }
        }
    }
}