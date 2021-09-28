#include "app.hpp"

#ifdef WIN32
#include <windows.h>
#endif

#include <filesystem>

#include "sane/logging/log.hpp"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

namespace
{
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

    unsigned int indices[] = {
        0, 1, 2, // first triangle
        2, 3, 0,  // second triangle
        4, 5, 6, // first triangle
        6, 7, 4  // second triangle
    };

    float vertices[] = {
         0.5f,  0.5f, -1.0f,   1.0f, 0.0f, 0.0f,
         0.5f, -0.5f, -1.0f,   0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, -1.0f,   0.0f, 0.0f, 1.0f,
        -0.5f,  0.5f, -1.0f,   1.0f, 1.0f, 0.0f
    };
}

namespace Sane
{
    App::App(const char* name)
        : evt_queue_(Events::Queue::Instance())
        , display_(name, WIDTH, HEIGHT)
        , imguiBegin_(&display_)
        , imguiEnd_(&display_)
        , sProg(vs_modern, fs_modern)
        , vbo(GL_ARRAY_BUFFER)
        , ibo(GL_ELEMENT_ARRAY_BUFFER)
        , framebuffer(WIDTH, HEIGHT)
        , vPos(sProg.GetAttribLocation("vPos"), 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0)
        , vCol(sProg.GetAttribLocation("vCol"), 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)))
    {
#if defined(WIN32)
        FreeConsole();
#endif
        PushLayer(&evt_queue_);
        layers_.PushImguiIntegration(&imguiBegin_, &imguiEnd_);
        SANE_WARN("Current path is {}", std::filesystem::current_path().string());
    }

    void App::Run()
    {
        while (display_.IsRunning())
        {
            framebuffer.Bind();
            framebuffer.Clear();
            {
                GLint mvp_location = sProg.GetUniformLocaition("MVP");

                {
                    sProg.Bind();

                    vbo.Bind();
                    vbo.BufferData(sizeof(vertices), vertices, GL_STATIC_DRAW);

                    ibo.Bind();
                    ibo.BufferData(sizeof(indices), indices, GL_STATIC_DRAW);

                    ibo.Unbind();
                    vbo.Unbind();

                    sProg.Unbind();

                    {
                        glm::mat4 p = glm::perspective(45.0f, 16.f / 9.f, 1.0f, 200.0f);

                        sProg.Bind();

                        vbo.Bind();
                        vPos.Enable();
                        vCol.Enable();

                        ibo.Bind();
                        glUniformMatrix4fv(mvp_location, 1, GL_FALSE, (const GLfloat*)&p[0][0]);
                        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);
                        ibo.Unbind();

                        vCol.Disable();
                        vPos.Disable();
                        vbo.Unbind();

                        sProg.Unbind();
                    }
                }
            }
            framebuffer.Unbind();

            framebuffer.Bind();
            {
                int width = framebuffer.GetSize().x;
                int height = framebuffer.GetSize().y;
                GLsizei nrChannels = 3;
                GLsizei stride = nrChannels * width;
                stride += (stride % 4) ? (4 - stride % 4) : 0;
                GLsizei bufferSize = stride * height;
                std::vector<char> buffer(bufferSize);
                glPixelStorei(GL_PACK_ALIGNMENT, 4);
                glReadBuffer(GL_FRONT);
                glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, buffer.data());
                stbi_flip_vertically_on_write(true);
                stbi_write_png("C:/temp/test.png", width, height, nrChannels, buffer.data(), stride);
            }
            framebuffer.Unbind();

            for (Layer* layer : layers_)
            {
                layer->Update();
            }

            display_.Update();
        }
    }

    void App::PushLayer(Layer* layer)
    {
        layers_.PushLayer(layer);
        SANE_INFO("Pushed layer on stack: {}", layer->Name());
    }

    void App::PushOverlay(Layer* layer)
    {
        layers_.PushOverlay(layer);
        SANE_INFO("Pushed overlay on stack: {}", layer->Name());
    }

    void App::PopLayer(Layer* layer)
    {
        layers_.PopLayer(layer);
    }

    void App::PopOverlay(Layer* layer)
    {
        layers_.PopOverlay(layer);
    }
}