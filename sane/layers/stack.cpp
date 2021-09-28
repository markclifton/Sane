#include "sane/layers/stack.hpp"

#include "sane/layers/layer.hpp"
#include "sane/logging/log.hpp"

namespace Sane
{
    namespace Layers
    {
        void Stack::PushLayer(Layer* layer)
        {
            layers_.emplace(layers_.begin() + (imguiIntegrationActive ? numLayers_ - 1 : numLayers_), layer);
            numLayers_++;
            layer->OnAttach();
        }

        void Stack::PushOverlay(Layer* overlay)
        {
            layers_.emplace(layers_.begin() + numLayers_, overlay);
            numOverlays_++;
            overlay->OnAttach();
        }

        void Stack::PopLayer(Layer* layer)
        {
            auto it = std::find(layers_.begin(), layers_.begin() + numLayers_, layer);
            if (it != layers_.begin() + numLayers_)
            {
                layers_.erase(it);
                numLayers_--;
                layer->OnDetach();
            }
            else
            {
                SANE_ERROR("Failed to pop layer {}", layer->name);
            }
        }

        void Stack::PopOverlay(Layer* overlay)
        {
            auto it = std::find(layers_.begin() + numLayers_, layers_.end(), overlay);
            if (it != layers_.end())
            {
                layers_.erase(it);
                numOverlays_--;
                overlay->OnDetach();
            }
            else
            {
                SANE_ERROR("Failed to pop overlay {}", overlay->name);
            }
        }

        void Stack::PushImguiIntegration(Layer* begin, Layer* end)
        {
            layers_.emplace(layers_.begin() + numLayers_, begin);
            layers_.emplace_back(end);

            numLayers_++;

            begin->OnAttach();
            end->OnAttach();

            imguiIntegrationActive = true;
        }

        void Stack::PopImguiIntegration(Layer* begin, Layer* end)
        {
            PopLayer(begin);
            PopOverlay(end);

            imguiIntegrationActive = false;
        }
    }
}