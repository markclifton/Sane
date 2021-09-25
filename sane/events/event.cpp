#include "sane/events/event.hpp"

namespace Sane
{
    Event::Event() {}

    Event::Event(uint32_t action, void* srcData, uint32_t size)
        : action(action), size(size)
    {
        data = malloc(size);
        memcpy(data, srcData, size);
    }

    Event::Event(uint32_t action, uint32_t detailedAction, void* srcData, uint32_t size)
        : action(action), detailedAction(detailedAction), size(size)
    {
        data = malloc(size);
        memcpy(data, srcData, size);
    }

    namespace Events
    {
        Listener::Listener(std::string name)
            : name_(name)
        {
            Queue::AddListener(this);
        }

        Listener::~Listener()
        {
            Queue::RemoveListener(this);
        }

        void Dispatcher::SubmitEvent(Event& event)
        {
            Queue::Instance().Submit(event);
        }

        Queue::Queue()
            : Layer("EventQueue")
        {
        }

        Queue& Queue::Instance()
        {
            static Queue queue;
            return queue;
        }

        void Queue::Submit(Event& evt)
        {
            std::lock_guard<std::mutex> lock(Instance().processingMutex_);
            Instance().events_.push_back(new Event(evt.action, evt.detailedAction, evt.data, evt.size));
        }

        void Queue::Update()
        {
            std::lock_guard<std::mutex> lock(Instance().processingMutex_);

            for (auto& event : Instance().events_)
            {
                for (auto& listener : Instance().listeners_)
                {
                    if (listener->ProcessEvent(*event))
                        break;
                }
                free(event->data);
            }

            Instance().events_.clear();
        }

        void Queue::AddListener(Listener* listener)
        {
            std::lock_guard<std::mutex> lock(Instance().processingMutex_);
            auto it = std::find(Instance().listeners_.begin(), Instance().listeners_.end(), listener);
            if (it == Instance().listeners_.end())
            {
                Instance().listeners_.push_back(listener);
            }
            else
            {
                SANE_WARN("Event listener already added: {}", listener->Name());
            }
        }

        void Queue::RemoveListener(Listener* listener)
        {
            std::lock_guard<std::mutex> lock(Instance().processingMutex_);
            auto it = std::find(Instance().listeners_.begin(), Instance().listeners_.end(), listener);
            if (it != Instance().listeners_.end())
            {
                Instance().listeners_.erase(it);
            }
            else
            {
                SANE_WARN("Event listener not found: {}", listener->Name());
            }
        }
    }
}