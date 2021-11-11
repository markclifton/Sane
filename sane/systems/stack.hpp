#pragma once

#include <string>
#include <vector>

namespace Sane
{
    class System;
    
    namespace Systems
    {
        class Stack
        {
        public:
            void PushLayer(System* layer);
            void PushOverlay(System* overlay);
            void PopLayer(System* layer);
            void PopOverlay(System* overlay);

            void PushImguiIntegration(System* begin, System* end);
            void PopImguiIntegration(System* begin, System* end);

            std::vector<System*>::iterator begin() { return systems_.begin(); }
            std::vector<System*>::iterator end() { return systems_.end(); }
            std::vector<System*>::reverse_iterator rbegin() { return systems_.rbegin(); }
            std::vector<System*>::reverse_iterator rend() { return systems_.rend(); }

            std::vector<System*>::const_iterator begin() const { return systems_.begin(); }
            std::vector<System*>::const_iterator end()	const { return systems_.end(); }
            std::vector<System*>::const_reverse_iterator rbegin() const { return systems_.rbegin(); }
            std::vector<System*>::const_reverse_iterator rend() const { return systems_.rend(); }

        protected:
            std::vector<System*> systems_;
            uint32_t numSystems_{ 0 };
            uint32_t numOverlays_{ 0 };
            bool imguiIntegrationActive{ false };
        };
    }
}