#include "sane/core/layer.hpp"

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
        layers.emplace(layers.begin() + insertIndex, layer);
        insertIndex++;
    }

    void LayerStack::PushOverlay(Layer* overlay)
    {
        layers.emplace_back(overlay);
    }

    void LayerStack::PopLayer(Layer* layer)
    {
        auto it = std::find(layers.begin(), layers.begin() + insertIndex, layer);
        if (it != layers.begin() + insertIndex)
        {
            layers.erase(it);
            insertIndex--;
        }
    }

    void LayerStack::PopOverlay(Layer* overlay)
    {
        auto it = std::find(layers.begin() + insertIndex, layers.end(), overlay);
        if (it != layers.end())
        {
            layers.erase(it);
        }
    }
}