#include "sane/core/event.hpp"

namespace Sane
{
    EventListener::EventListener(std::string name)
        : name(name)
    {
        EventQueue::AddListener(this);
    }

    EventListener::~EventListener()
    {
        EventQueue::RemoveListener(this);
    }

    void EventDispatcher::SubmitEvent(Event& event)
    {
        EventQueue::Instance().Submit(event);
    }

    EventQueue::EventQueue()
        : Layer("EventQueue")
    {
    }

    EventQueue& EventQueue::Instance()
    {
        static EventQueue queue;
        return queue;
    }

    void EventQueue::Submit(Event& evt)
    {
        std::lock_guard<std::mutex> lock(Instance().processingMutex);

        Event* event = new Event();
        event->id = evt.id;
        event->data = malloc(evt.size);
        memcpy(event->data, evt.data, evt.size);

        Instance().events.push_back(event);
    }

    void EventQueue::Process()
    {
        std::lock_guard<std::mutex> lock(Instance().processingMutex);

        for (auto& event : Instance().events)
        {
            for (auto& listener : Instance().listeners)
            {
                if (listener->ProcessEvent(*event))
                    break;
            }
            free(event->data);
        }

        Instance().events.clear();
    }

    void EventQueue::AddListener(EventListener* listener)
    {
        std::lock_guard<std::mutex> lock(Instance().processingMutex);
        auto it = std::find(Instance().listeners.begin(), Instance().listeners.end(), listener);
        if (it == Instance().listeners.end())
        {
            Instance().listeners.push_back(listener);
        }
        else
        {
            SANE_WARN("Event listener already added: {}", listener->Name());
        }
    }

    void EventQueue::RemoveListener(EventListener* listener)
    {
        std::lock_guard<std::mutex> lock(Instance().processingMutex);
        auto it = std::find(Instance().listeners.begin(), Instance().listeners.end(), listener);
        if (it == Instance().listeners.end())
        {
            SANE_WARN("Event listener not found: {}", listener->Name());
        }
        else
        {
            Instance().listeners.erase(it);
        }
    }
}