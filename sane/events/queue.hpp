#pragma once

#include <mutex>
#include <vector>

#include "sane/events/event.hpp"
#include "sane/events/listener.hpp"
#include "sane/systems/system.hpp"

namespace Sane
{
    namespace Events
    {
        class Queue : public System
        {
            Queue();
            friend class Listener;
            friend class Dispatcher;

        public:
            static Queue& Instance();

            static void Submit(Event& evt);
            virtual void Update(double ts);

        private:
            static void AddListener(Listener* listener);
            static void RemoveListener(Listener* listener);

            std::mutex processingMutex_;
            std::vector<Event*> events_;
            std::vector<Listener*> listeners_;
        };
    }
}