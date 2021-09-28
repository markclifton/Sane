#include "sane/layers/gamewindow.hpp"

#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <glm/ext.hpp>
#include <glm/glm.hpp>

#include "imgui.h"

namespace Sane
{
    GameWindow::GameWindow(uint32_t framebufferId)
        : Sane::Layer("GameWindow")
        , frameSize(1280, 720)
        , framebufferId(framebufferId)
    {}

    void GameWindow::Update()
    {
        frameSize = ImGui::GetWindowSize();

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.f, 0.f });
        ImGui::Begin("GameWindow");
        {
            ImGui::BeginChild("GameRender");
            ImGui::Image((void*)(intptr_t)framebufferId, frameSize, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
            ImGui::EndChild();
        }
        ImGui::End();
        ImGui::PopStyleVar();
    }
}