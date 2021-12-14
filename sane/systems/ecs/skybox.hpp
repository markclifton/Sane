#pragma once

#include "sane/ecs/common.hpp"
#include "sane/events/common.hpp"
#include "sane/graphics/common.hpp"
#include "sane/systems/ecs/base.hpp"

namespace
{
    static const char* vs_grid = R""(
        #version 450                                                                                                
        layout (location = 0) in vec3 vPos;
        layout (location = 1) in vec2 vUV;
        out vec2 fs_vUV;
        void main()
        {
        gl_Position = MVP * vec4(vPos, 1.0);
            UV_CS_in = vUV;
        }
    )"";

    static const char* fs_grid = R""(
        #version 450 core
        out vec4 FragColor;
        in vec2 fs_vUV;
        uniform sampler2D tex;
        void main()
        {
            FragColor = texture(tex, fs_vUV);
        }
    )"";
}

namespace Sane
{
    namespace ECS
    {
        class Skybox : public SystemBase
        {
            ShaderProgram sProg;
            Buffer vertices_buffer;
            Buffer indices_buffer;

        public:
            Skybox(entt::registry& registry)
                : SystemBase("SkyboxSystem", registry)
                , sProg()
                , vertices_buffer(GL_ARRAY_BUFFER)
                , indices_buffer(GL_ELEMENT_ARRAY_BUFFER)
            {
                sProg.AddVertexShader(vs_grid);
                sProg.AddFragmentShader(fs_grid);
                sProg.Finalize();
            }

            virtual void RenderScene(glm::mat4 mvp) override
            {
                auto view = registry_.view<Components::RenderContext, Components::Camera, Components::Position>();
                view.each([&](const auto entity, const Components::RenderContext& context, const Components::Camera& camera, const Components::Position& position) {
                    if (!camera.enabled)
                        return;

                    GLint old;
                    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &old);

                    glBindFramebuffer(GL_FRAMEBUFFER, context.framebuffer);
                    glViewport(0, 0, context.width, context.height);

                    GLint OldCullFaceMode;
                    glGetIntegerv(GL_CULL_FACE_MODE, &OldCullFaceMode);
                    GLint OldDepthFuncMode;
                    glGetIntegerv(GL_DEPTH_FUNC, &OldDepthFuncMode);

                    glCullFace(GL_FRONT);
                    glDepthFunc(GL_LEQUAL);

                    sProg.Bind();
                    vertices_buffer.Bind();
                    indices_buffer.Bind();



                    indices_buffer.Unbind();
                    vertices_buffer.Unbind();
                    sProg.Unbind();

                    glCullFace(OldCullFaceMode);
                    glDepthFunc(OldDepthFuncMode);

                    glBindFramebuffer(GL_FRAMEBUFFER, old);

                    });
            }

            virtual void OnAttach() override
            {
                std::vector<float> verts;
                std::vector<uint32_t> inds;

                sProg.Bind();

                vertices_buffer.Bind();
                vertices_buffer.BufferData(sizeof(float) * verts.size(), &verts[0], GL_STATIC_DRAW);

                indices_buffer.Bind();
                indices_buffer.BufferData(sizeof(uint32_t) * inds.size(), &inds[0], GL_STATIC_DRAW);

                indices_buffer.Unbind();
                vertices_buffer.Unbind();

                sProg.Unbind();
            }

        private:
        };
    }
}