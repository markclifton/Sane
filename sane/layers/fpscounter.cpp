#include "sane/layers/fpscounter.hpp"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

namespace Sane
{
    FpsCounter::FpsCounter()
        : Layer("FpsCounter")
    {
    }

    void FpsCounter::Update()
    {
        if (ImGui::Begin("Framerate"))
        {
            ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        }
        ImGui::End();
    }
}