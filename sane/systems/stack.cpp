#include "sane/systems/stack.hpp"

#include "sane/logging/log.hpp"
#include "sane/systems/system.hpp"

namespace Sane
{
    namespace Systems
    {
        void Stack::PushLayer(System* sys)
        {
            systems_.emplace(systems_.begin() + (imguiIntegrationActive ? numSystems_ - 1 : numSystems_), sys);
            numSystems_++;
            sys->OnAttach();
        }

        void Stack::PushOverlay(System* sys)
        {
            systems_.emplace(systems_.begin() + numSystems_, sys);
            numOverlays_++;
            sys->OnAttach();
        }

        void Stack::PopLayer(System* sys)
        {
            auto it = std::find(systems_.begin(), systems_.begin() + numSystems_, sys);
            if (it != systems_.begin() + numSystems_)
            {
                systems_.erase(it);
                numSystems_--;
                sys->OnDetach();
            }
            else
            {
                SANE_ERROR("Failed to pop system {}", sys->name);
            }
        }

        void Stack::PopOverlay(System* sys)
        {
            auto it = std::find(systems_.begin() + numSystems_, systems_.end(), sys);
            if (it != systems_.end())
            {
                systems_.erase(it);
                numOverlays_--;
                sys->OnDetach();
            }
            else
            {
                SANE_ERROR("Failed to pop system {}", sys->name);
            }
        }

        void Stack::PushImguiIntegration(System* begin, System* end)
        {
            systems_.emplace(systems_.begin() + numSystems_, begin);
            systems_.emplace_back(end);

            numSystems_++;

            begin->OnAttach();
            end->OnAttach();

            imguiIntegrationActive = true;
        }

        void Stack::PopImguiIntegration(System* begin, System* end)
        {
            PopLayer(begin);
            PopOverlay(end);

            imguiIntegrationActive = false;
        }
    }
}