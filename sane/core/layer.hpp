#pragma once

#include <string>
#include <vector>

namespace Sane
{
    class Layer
    {
    public:
        Layer(const std::string& name = "NewLayer");
        virtual ~Layer() = default;

        virtual void Process() = 0;

        const std::string& Name() const { return name; }
    protected:
        std::string name;
    };

    class LayerStack
    {
    public:
        LayerStack() = default;
        ~LayerStack();

        void PushLayer(Layer* layer);
        void PushOverlay(Layer* overlay);
        void PopLayer(Layer* layer);
        void PopOverlay(Layer* overlay);

        std::vector<Layer*>::iterator begin() { return layers.begin(); }
        std::vector<Layer*>::iterator end() { return layers.end(); }
        std::vector<Layer*>::reverse_iterator rbegin() { return layers.rbegin(); }
        std::vector<Layer*>::reverse_iterator rend() { return layers.rend(); }

        std::vector<Layer*>::const_iterator begin() const { return layers.begin(); }
        std::vector<Layer*>::const_iterator end()	const { return layers.end(); }
        std::vector<Layer*>::const_reverse_iterator rbegin() const { return layers.rbegin(); }
        std::vector<Layer*>::const_reverse_iterator rend() const { return layers.rend(); }
    private:
        std::vector<Layer*> layers;
        unsigned int insertIndex = 0;
    };
}