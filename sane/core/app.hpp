#pragma once

#include "sane/core/display.hpp"
#include "sane/events/event.hpp"
#include "sane/graphics/buffer.hpp"
#include "sane/graphics/framebuffer.hpp"
#include "sane/graphics/shaderprogram.hpp"
#include "sane/layers/base.hpp"
#include "sane/layers/imgui.hpp"

#include <iostream>

namespace Sane
{
    class App
    {
    public:
        App(const char* name);
        virtual ~App() = default;
        void Run();

        void PrintLayers()
        {
            for (auto& layer : layers_)
            {
                std::cout << layer->Name() << " ";
            }
            std::cout << "\n";
        }

        void PushLayer(Layer* layer);
        void PushOverlay(Layer* layer);
        void PopLayer(Layer* layer);
        void PopOverlay(Layer* layer);

    private:
        const int32_t WIDTH = 1280;
        const int32_t HEIGHT = 720;

        Events::Queue& evt_queue_;
        Layers::Stack layers_;

        Display display_;
        ImguiBeginLayer imguiBegin_;
        ImguiEndLayer imguiEnd_;

        //TEST CODE
        ShaderProgram sProg;
        Buffer vbo;
        Buffer ibo;
        VertexAttrib vPos;
        VertexAttrib vCol;

    public:
        Framebuffer framebuffer;
    };

    App* CreateApp();
}