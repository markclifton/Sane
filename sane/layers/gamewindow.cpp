#include "sane/layers/gamewindow.hpp"

#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <glm/ext.hpp>
#include <glm/glm.hpp>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

namespace Sane
{
    GameWindow::GameWindow(uint32_t framebufferId)
        : Sane::Layer("GameWindow")
        , frameSize(400, 400)
        , framebufferId(framebufferId)
    {}

    void GameWindow::Update()
    {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.f, 0.f });
        ImGui::Begin("GameWindow");
        {
            ImGui::BeginChild("GameRender");
            ImGui::Image(reinterpret_cast<ImTextureID>(framebufferId), ImGui::GetWindowSize(), ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
            ImGui::EndChild();
        }
        ImGui::End();
        ImGui::PopStyleVar();
    }
}