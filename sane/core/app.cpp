#include "app.hpp"

#ifdef WIN32
#include <windows.h>
#endif

#include <filesystem>

#include "sane/logging/log.hpp"

namespace Sane
{
    App::App(const char* name)
        : evt_queue_(Events::Queue::Instance())
        , display_(name, WIDTH, HEIGHT)
        , imgui_(&display_)
    {
#if defined(WIN32)
        FreeConsole();
#endif
        PushLayer(&evt_queue_);
        PushLayer(&imgui_);
    }

    void App::Run()
    {
        SANE_WARN("Current path is {}", std::filesystem::current_path().c_str());

        while (display_.IsRunning())
        {
            for (Layer* layer : layers_)
            {
                layer->Update();
            }

            imgui_.Begin();
            {
                for (Layer* layer : layers_)
                    layer->Render();
            }
            imgui_.End();

            display_.Update();
        }
    }

    void App::PushLayer(Layer* layer)
    {
        layers_.PushLayer(layer);
        layer->OnAttach();
    }

    void App::PushOverlay(Layer* layer)
    {
        layers_.PushOverlay(layer);
        layer->OnAttach();
    }

    void App::PopLayer(Layer* layer)
    {
        layers_.PopLayer(layer);
        layer->OnDetach();
    }

    void App::PopOverlay(Layer* layer)
    {
        layers_.PopOverlay(layer);
        layer->OnDetach();
    }
}