#include "app.hpp"

#include <filesystem>

#ifdef WIN32
#include <windows.h>
#endif

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

#include "sane/ecs/common.hpp"
#include "sane/logging/log.hpp"
#include "sane/systems/common.hpp"
#include "sane/systems/ecs/scene.hpp"

const int32_t WIDTH = 2560;
const int32_t HEIGHT = 1440;

namespace Sane
{
    App::App(const char* name)
        : evt_queue_(Events::Queue::Instance())
        , display_(name, WIDTH, HEIGHT)
        , imguiBegin_(&display_)
        , imguiEnd_(&display_)
        , framebuffer(WIDTH, HEIGHT)
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
        {
            const auto camera0 = Registry().create();
            Registry().emplace<Sane::Components::Camera>(camera0, true, glm::mat4(1.f), glm::vec3(0.f, 0.f, 0.f), 10.0f);
            Registry().emplace<Sane::Components::Position>(camera0, glm::vec4(2.f, -4.f, 0.f, 0.f));
            Registry().emplace<Sane::Components::Rotation>(camera0, 0.f, 0.f, 0.f);
            Registry().emplace<Sane::Components::RenderContext>(camera0, WIDTH, HEIGHT);
            Registry().emplace<Sane::Components::Player>(camera0, true);
        }

        ECS::Scene scene(Registry());
        ECS::Camera camSystem(Registry());
        ECS::Grid grid(Registry(), 16);
        //ECS::Projectile proj(Registry());
        PushLayer(&grid);
        //PushLayer(&proj);
        PushLayer(&scene);
        PushLayer(&camSystem);

        double last = glfwGetTime();
        while (display_.IsRunning())
        {
            for (auto& layer : layers_)
            {
                layer->Update((glfwGetTime() - last) * 1000);
            }
            last = glfwGetTime();

            framebuffer.Bind();
            framebuffer.Clear();
            for (auto& layer : layers_)
            {
                layer->RenderScene(display_.GetPersProjection());
            }
            framebuffer.Unbind();

            for (auto& layer : layers_)
            {
                layer->RenderGui();
            }

            display_.Update();
        }
    }

    void App::PushLayer(System* layer)
    {
        layers_.PushLayer(layer);
        SANE_INFO("Pushed layer on stack: {}", layer->name);
    }

    void App::PushOverlay(System* layer)
    {
        layers_.PushOverlay(layer);
        SANE_INFO("Pushed overlay on stack: {}", layer->name);
    }

    void App::PopLayer(System* layer)
    {
        layers_.PopLayer(layer);
    }

    void App::PopOverlay(System* layer)
    {
        layers_.PopOverlay(layer);
    }
}