#include "sane/systems/gui/gamewindow.hpp"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

namespace Sane
{
    GameWindow::GameWindow(uint32_t framebufferId)
        : System("GameWindow")
        , frameSize_(3840, 2160)
        , framebufferId_(framebufferId)
    {}

    void GameWindow::RenderGui()
    {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.f, 0.f });
        if (ImGui::Begin("GameWindow"))
        {
            ImGui::BeginChild("GameRender");
            ImGui::Image(reinterpret_cast<ImTextureID>(framebufferId_), ImGui::GetWindowSize(), ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
            ImGui::EndChild();
        }
        ImGui::End();
        ImGui::PopStyleVar();
    }
}