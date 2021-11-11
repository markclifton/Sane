#include "sane/systems/system.hpp"

namespace Sane
{
    System::System(const std::string& name)
        : name(name)
    {}

    void System::Update(double ts) {}
    void System::RenderScene(glm::mat4 mvp) {}
    void System::RenderGui() {}

    void System::OnAttach() {}
    void System::OnDetach() {}
}