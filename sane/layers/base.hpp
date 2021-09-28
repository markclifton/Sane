#pragma once

#include <string>
#include <vector>

namespace Sane
{
    namespace Layers
    {
        class Stack;
    }

    class Layer
    {
        friend class Layers::Stack;
    public:
        Layer(const std::string& name);

        virtual void Update() {}

        virtual void OnAttach() {}
        virtual void OnDetach() {}

        const std::string& Name() const { return name; }

    protected:
        std::string name;
    };

    namespace Layers
    {
        class Stack
        {
        public:
            void PushLayer(Layer* layer);
            void PushOverlay(Layer* overlay);
            void PopLayer(Layer* layer);
            void PopOverlay(Layer* overlay);

            void PushImguiIntegration(Layer* begin, Layer* end);
            void PopImguiIntegration(Layer* begin, Layer* end);

            std::vector<Layer*>::iterator begin() { return layers_.begin(); }
            std::vector<Layer*>::iterator end() { return layers_.end(); }
            std::vector<Layer*>::reverse_iterator rbegin() { return layers_.rbegin(); }
            std::vector<Layer*>::reverse_iterator rend() { return layers_.rend(); }

            std::vector<Layer*>::const_iterator begin() const { return layers_.begin(); }
            std::vector<Layer*>::const_iterator end()	const { return layers_.end(); }
            std::vector<Layer*>::const_reverse_iterator rbegin() const { return layers_.rbegin(); }
            std::vector<Layer*>::const_reverse_iterator rend() const { return layers_.rend(); }

        protected:
            std::vector<Layer*> layers_;
            uint32_t numLayers_{ 0 };
            uint32_t numOverlays_{ 0 };
            bool imguiIntegrationActive{ false };
        };
    }
}