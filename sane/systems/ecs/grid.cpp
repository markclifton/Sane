#include "sane/systems/ecs/grid.hpp"

#include "sane/ecs/common.hpp"
#include "sane/logging/log.hpp"

namespace
{
    static const char* vs_grid = R""(
        #version 450                                                                                                
        layout (location = 0) in vec3 vPos;
        layout (location = 1) in vec2 vUV;
        out vec3 WorldPos_CS_in;
        out vec2 UV_CS_in;
        uniform mat4 model;                                                                                    
        void main()
        {
            WorldPos_CS_in = (model * vec4(vPos, 1.f)).xyz;
            UV_CS_in = vUV;
        }
    )"";

    static const char* tcs_grid = R""(
        #version 450 core
        layout (vertices = 3) out;
        uniform vec3 gEyeWorldPos;
        in vec3 WorldPos_CS_in[];
        in vec2 UV_CS_in[];
        out vec3 WorldPos_ES_in[];
        out vec2 UV_ES_in[];
        float GetTessLevel(float Distance0, float Distance1)
        {
            return 32.f;
        }
        void main()
        {
            UV_ES_in[gl_InvocationID] = UV_CS_in[gl_InvocationID];
            WorldPos_ES_in[gl_InvocationID] = WorldPos_CS_in[gl_InvocationID];
            float EyeToVertexDistance0 = distance(gEyeWorldPos, WorldPos_ES_in[0]);
            float EyeToVertexDistance1 = distance(gEyeWorldPos, WorldPos_ES_in[1]);
            float EyeToVertexDistance2 = distance(gEyeWorldPos, WorldPos_ES_in[2]);
            gl_TessLevelOuter[0] = GetTessLevel(EyeToVertexDistance1, EyeToVertexDistance2);
            gl_TessLevelOuter[1] = GetTessLevel(EyeToVertexDistance2, EyeToVertexDistance0);
            gl_TessLevelOuter[2] = GetTessLevel(EyeToVertexDistance0, EyeToVertexDistance1);
            gl_TessLevelInner[0] = gl_TessLevelOuter[2];
        }
    )"";

    static const char* tes_grid = R""(
        #version 450 core
        layout(triangles, equal_spacing, cw) in;
        in vec3 WorldPos_ES_in[];
        in vec2 UV_ES_in[];
        out vec2 UV_FS_in;
        uniform mat4 MVP;
        uniform mat4 model;                                                                                    
        uniform sampler2D tex;
        vec2 interpolate2D(vec2 v0, vec2 v1, vec2 v2)                                                   
        {
            return vec2(gl_TessCoord.x) * v0 + vec2(gl_TessCoord.y) * v1 + vec2(gl_TessCoord.z) * v2;
        }
        vec3 interpolate3D(vec3 v0, vec3 v1, vec3 v2)
        {
            return vec3(gl_TessCoord.x) * v0 + vec3(gl_TessCoord.y) * v1 + vec3(gl_TessCoord.z) * v2;
        }
        void main()
        {
            UV_FS_in = interpolate2D(UV_ES_in[0], UV_ES_in[1], UV_ES_in[2]);    
            float Displacement = texture(tex, UV_FS_in.xy).x;
            vec3 worldPos = interpolate3D(WorldPos_ES_in[0], WorldPos_ES_in[1], WorldPos_ES_in[2]);
            worldPos.y += 125.f * Displacement;  
            gl_Position = MVP * vec4(worldPos, 1.0);
        }
    )"";

    static const char* fs_grid = R""(
        #version 450 core
        out vec4 FragColor;
        in vec2 UV_FS_in;
        uniform sampler2D tex;
        void main()
        {
            FragColor = texture(tex, UV_FS_in);
        }
    )"";

    constexpr float vertices[] = {
        -1.0,  1.0, 0.0, 0, 1,
         1.0,  1.0, 0.0, 1, 1,
         1.0, -1.0, 0.0, 1, 0,
         1.0, -1.0, 0.0, 1, 0,
        -1.0, -1.0, 0.0, 0, 0,
        -1.0,  1.0, 0.0, 0, 1,
    };
    constexpr uint32_t indices[] = { 0,1,2,2,3,0 };
}

namespace Sane
{
    namespace ECS
    {
        Grid::Grid(entt::registry& registry)
            : SystemBase("GridSystem", registry)
            , sProg()
            , vertices_buffer(GL_ARRAY_BUFFER)
            , indices_buffer(GL_ELEMENT_ARRAY_BUFFER)
            , vPos(sProg.GetAttribLocation("vPos"), 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0)
            , vUV(sProg.GetAttribLocation("vUV"), 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(sizeof(float) * 3))
        {
            sProg.AddVertexShader(vs_grid);
            sProg.AddTessControlShader(tcs_grid);
            sProg.AddTessEvaluationShader(tes_grid);
            sProg.AddFragmentShader(fs_grid);
            sProg.Finalize();
        }

        void Grid::RenderScene(glm::mat4 mvp)
        {
            auto view = registry_.view<Components::RenderContext, Components::Camera, Components::Position>();
            view.each([&](const auto entity, const Components::RenderContext& context, const Components::Camera& camera, const Components::Position& position) {
                if (!camera.enabled)
                    return;

                GLint old;
                glGetIntegerv(GL_FRAMEBUFFER_BINDING, &old);

                glBindFramebuffer(GL_FRAMEBUFFER, context.framebuffer);
                glViewport(0, 0, context.width, context.height);

                glClearColor(.2f, .3f, .8f, 1.f);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                sProg.Bind();

                vertices_buffer.Bind();
                vPos.Enable();
                vUV.Enable();

                mvp *= camera.lookat;

                glm::mat4 trans = glm::mat4(1.f); //glm::translate(glm::mat4(1.f), { position.data.x, -(position.data.y + 4), position.data.z });
                glm::mat4 model_matrix = trans * glm::rotate(glm::mat4(1.f), 1.57079633f, { 1.f, 0.f, 0.f }) * glm::scale(glm::mat4(1.f), { 500, 500, 0 });

                glUniformMatrix4fv(sProg.GetUniformLocaition("MVP"), 1, GL_FALSE, (const GLfloat*)&mvp[0][0]);
                glUniform3f(sProg.GetUniformLocaition("gEyeWorldPos"), position.data.x, position.data.y, position.data.z);

                glUniformMatrix4fv(sProg.GetUniformLocaition("model"), 1, GL_FALSE, (const GLfloat*)&model_matrix[0][0]);

                //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                static Texture heightmap("img/heightmap.png");
                glBindTexture(GL_TEXTURE_2D, heightmap.GetTextureId());

                indices_buffer.Bind();
                glDrawElements(GL_PATCHES, 6, GL_UNSIGNED_INT, (void*)0);
                indices_buffer.Unbind();

                glBindTexture(GL_TEXTURE_2D, 0);
                //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

                vUV.Disable();
                vPos.Disable();
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
            vertices_buffer.BufferData(sizeof(vertices), &vertices[0], GL_STATIC_DRAW);

            indices_buffer.Bind();
            indices_buffer.BufferData(sizeof(indices), &indices[0], GL_STATIC_DRAW);

            indices_buffer.Unbind();
            vertices_buffer.Unbind();

            sProg.Unbind();
        }
    }
}