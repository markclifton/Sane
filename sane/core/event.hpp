#pragma once

#include <mutex>
#include <vector>

#include <sane/debugging/logging.hpp>
#include <sane/core/layer.hpp>

namespace Sane
{
    class EventQueue;

    struct Event
    {
        unsigned int id;
        void* data;
        size_t size;
    };

    class EventListener
    {
    public:
        EventListener(std::string name);
        ~EventListener();

        virtual bool ProcessEvent(Event& evt) = 0;

        const std::string Name() { return name; }

    private:
        const std::string name;
    };

    class EventDispatcher
    {
    public:
        void SubmitEvent(Event& event);
    };

    class EventQueue : public Layer
    {
        EventQueue();
        friend class EventListener;
        friend class EventSubmitter;

    public:
        static EventQueue& Instance();

        static void Submit(Event& evt);
        virtual void Process() override;

    public:
        static void AddListener(EventListener* listener);
        static void RemoveListener(EventListener* listener);

    private:
        std::mutex processingMutex;
        std::vector<Event*> events;
        std::vector<EventListener*> listeners;
    };
}