#pragma once

#include <mutex>
#include <vector>

#include "sane/events/event.hpp"
#include "sane/layers/layer.hpp"

namespace Sane
{
    namespace Events
    {
        class Queue : public Layer
        {
            Queue();
            friend class Listener;
            friend class Dispatcher;

        public:
            static Queue& Instance();

            static void Submit(Event& evt);
            virtual void Update() override;

        private:
            static void AddListener(Listener* listener);
            static void RemoveListener(Listener* listener);

            std::mutex processingMutex_;
            std::vector<Event*> events_;
            std::vector<Listener*> listeners_;
        };
    }
}