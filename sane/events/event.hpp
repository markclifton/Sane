#pragma once

#include <mutex>
#include <vector>

#include "sane/layers/layer.hpp"
#include "sane/logging/log.hpp"

namespace Sane
{
    struct Event
    {
        uint32_t action{ 0 };
        uint32_t detailedAction{ 0 };
        void* data{ nullptr };
        uint32_t size{ 0 };

        Event();
        Event(uint32_t action, void* srcData, uint32_t size);
        Event(uint32_t action, uint32_t detailedAction, void* srcData, uint32_t size);
    };

    namespace Events
    {
        class Queue;

        class Listener
        {
        public:
            Listener(std::string name);
            ~Listener();

            virtual bool ProcessEvent(Event& evt) = 0;

            inline const std::string Name() { return name_; }
        private:
            const std::string name_;
        };

        class Dispatcher
        {
        public:
            void SubmitEvent(Event& event);
        };

        class Queue : public Layer
        {
            Queue();
            friend class Listener;
            friend class Dispatcher;

        public:
            static Queue& Instance();

            static void Submit(Event& evt);
            virtual void Process() override;

        private:
            static void AddListener(Listener* listener);
            static void RemoveListener(Listener* listener);

            std::mutex processingMutex_;
            std::vector<Event*> events_;
            std::vector<Listener*> listeners_;
        };
    }
}