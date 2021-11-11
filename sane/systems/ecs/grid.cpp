#include "sane/systems/ecs/grid.hpp"

#include "sane/ecs/common.hpp"

namespace
{
    static const char* vs_grid = R""(
    #version 330 core
    layout (location = 1) in vec3 vPos;
    out vec2 fragCoord;
    uniform mat4 MVP;
    void main()
    {
        fragCoord = vPos.xy;
        gl_Position = MVP * vec4(vPos, 1.0);
    }
    )"";

    static const char* fs_grid = R""(
    #version 330 core
    in vec2 fragCoord;
    out vec4 fragColor;
    const float PI = 3.1415926535897932384626433832795;
    const vec2 CANVAS_SIZE = vec2(2);
    vec2 convert_to_cell_coords(vec2 coord, vec2 grid);
    float lineWidth = 0.001;
    vec2 grid = vec2(32);
    vec2 cellSize = CANVAS_SIZE / grid;
    void main()
    {
        vec2 cellCoord = convert_to_cell_coords(fragCoord, cellSize);
        vec2 cutoff = convert_to_cell_coords(vec2(1.0 - lineWidth), cellSize);

        vec2 alpha = step(cutoff, cellCoord);
        if (max(alpha.x, alpha.y) == 0.0)
            discard;

        fragColor = vec4(1.0, 1.0, 1.0, 1.0);
    }
    vec2 convert_to_cell_coords(vec2 coord, vec2 cellSize)
    {
        return cos(((2 * PI) / cellSize) * coord);
    }
    )"";

    constexpr float vertices[] = { -1.0,  1.0, 0.0, -1.0, -1.0, 0.0, 1.0, -1.0, 0.0, 1.0,  1.0, 0.0 };
}


namespace Sane
{
    namespace ECS
    {
        Grid::Grid(entt::registry& registry)
            : SystemBase("GridSystem", registry)
            , sProg(vs_grid, fs_grid)
            , vertices_buffer(GL_ARRAY_BUFFER)
            , vPos(sProg.GetAttribLocation("vPos"), 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0)
        {
        }

        void Grid::RenderScene(glm::mat4 mvp)
        {

            auto view = registry_.view<Components::RenderContext, Components::Camera>();
            view.each([&](const auto entity, const Components::RenderContext& context, const Components::Camera& camera) {
                if (!camera.enabled)
                    return;

                GLint old;
                glGetIntegerv(GL_FRAMEBUFFER_BINDING, &old);

                glBindFramebuffer(GL_FRAMEBUFFER, context.framebuffer);
                glViewport(0, 0, context.width, context.height);

                sProg.Bind();

                vertices_buffer.Bind();
                vPos.Enable();

                mvp *= camera.lookat;
                mvp = glm::rotate(mvp, 1.57079633f, { 1.f, 0.f, 0.f });
                mvp = glm::scale(mvp, { 100, 100, 0 });

                glUniformMatrix4fv(sProg.GetUniformLocaition("MVP"), 1, GL_FALSE, (const GLfloat*)&mvp[0][0]);

                glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

                vertices_buffer.Unbind();
                sProg.Unbind();

                glBindFramebuffer(GL_FRAMEBUFFER, old);
                }
            );
        }

        void Grid::OnAttach()
        {
            sProg.Bind();

            vertices_buffer.Bind();
            vertices_buffer.BufferData(sizeof(float) * 12, &vertices[0], GL_STATIC_DRAW);

            sProg.Unbind();
        }
    }
}