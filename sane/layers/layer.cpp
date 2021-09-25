#include "sane/layers/layer.hpp"

namespace Sane
{
    Layer::Layer(const std::string& name)
        : name(name)
    {
    }

    LayerStack::~LayerStack()
    {
    }

    void LayerStack::PushLayer(Layer* layer)
    {
        layers_.emplace(layers_.begin(), layer);
    }

    void LayerStack::PopLayer(Layer* layer)
    {
        auto it = std::find(layers_.begin(), layers_.begin(), layer);
        if (it != layers_.begin())
        {
            layers_.erase(it);
        }
    }
}