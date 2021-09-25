#include "sane/layers/fpscounter.hpp"

#include <imgui.h>

namespace Sane
{
    void FpsCounter::Process()
    {
        if (ImGui::Begin("Framerate"))
        {
            ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        }
        ImGui::End();
    }
}