#pragma once

#include <string>
#include <vector>

namespace Sane
{
    class Layer
    {
    public:
        Layer(const std::string& name = "NewLayer");

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
        void PopLayer(Layer* layer);

        std::vector<Layer*>::iterator begin() { return layers_.begin(); }
        std::vector<Layer*>::iterator end() { return layers_.end(); }
        std::vector<Layer*>::reverse_iterator rbegin() { return layers_.rbegin(); }
        std::vector<Layer*>::reverse_iterator rend() { return layers_.rend(); }

        std::vector<Layer*>::const_iterator begin() const { return layers_.begin(); }
        std::vector<Layer*>::const_iterator end()	const { return layers_.end(); }
        std::vector<Layer*>::const_reverse_iterator rbegin() const { return layers_.rbegin(); }
        std::vector<Layer*>::const_reverse_iterator rend() const { return layers_.rend(); }
    private:
        std::vector<Layer*> layers_;
        uint32_t layerInsertIndex{ 0 };
    };
}