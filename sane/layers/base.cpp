#include "sane/layers/base.hpp"

#include "sane/logging/log.hpp"

namespace Sane
{
    Layer::Layer(const std::string& name)
        : name(name)
    {
    }

    namespace Layers
    {
        void Stack::PushLayer(Layer* layer)
        {
            layers_.emplace(layers_.begin() + layerInsertIndex_, layer);
            layerInsertIndex_++;
        }

        void Stack::PushOverlay(Layer* overlay)
        {
            layers_.emplace_back(overlay);
        }

        void Stack::PopLayer(Layer* layer)
        {
            auto it = std::find(layers_.begin(), layers_.begin() + layerInsertIndex_, layer);
            if (it != layers_.begin() + layerInsertIndex_)
            {
                layers_.erase(it);
                layerInsertIndex_--;
            }
            else
            {
                SANE_ERROR("Failed to pop layer {}", layer->name);
            }
        }

        void Stack::PopOverlay(Layer* overlay)
        {
            auto it = std::find(layers_.begin() + layerInsertIndex_, layers_.end(), overlay);
            if (it != layers_.end())
            {
                layers_.erase(it);
            }
            else
            {
                SANE_ERROR("Failed to pop overlay {}", overlay->name);
            }
        }
    }
}