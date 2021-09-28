#pragma once

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "sane/core/display.hpp"
#include "sane/layers/base.hpp"

namespace Sane
{
    class ImguiBeginLayer : public Layer
    {
        Sane::Display* display_;
    public:
        ImguiBeginLayer(Sane::Display* display)
            : Layer("ImguiBeginLayer")
            , display_(display)
        {
        }

        virtual void OnAttach() override
        {
            IMGUI_CHECKVERSION();
            ImGui::CreateContext();
            ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;
            ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
            ImGui::StyleColorsDark();
            ImGui_ImplGlfw_InitForOpenGL(*display_, true);
            ImGui_ImplOpenGL3_Init(nullptr);
        }

        virtual void OnDetach() override
        {
            ImGui_ImplOpenGL3_Shutdown();
            ImGui_ImplGlfw_Shutdown();
            ImGui::DestroyContext();
        }

        virtual void Update() override
        {
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();
            ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
        }
    };

    class ImguiEndLayer : public Layer
    {
        Sane::Display* display_;
    public:
        ImguiEndLayer(Sane::Display* display)
            : Layer("ImguiEndLayer")
            , display_(display)
        {
        }

        virtual void Update() override
        {
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
            {
                ImGui::UpdatePlatformWindows();
                ImGui::RenderPlatformWindowsDefault();
                glfwMakeContextCurrent(*display_);
            }
        }
    };
}