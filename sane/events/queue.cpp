#include "sane/events/queue.hpp"

#include "sane/events/listener.hpp"
#include "sane/logging/log.hpp"

namespace Sane
{
    namespace Events
    {
        Queue::Queue()
            : System("EventQueue")
        {
        }

        Queue& Queue::Instance()
        {
            static Queue queue;
            return queue;
        }

        void Queue::Submit(std::unique_ptr<Event> evt)
        {
            std::lock_guard<std::mutex> lock(Instance().processingMutex_);
            Instance().events_.push_back(std::move(evt));
        }

        void Queue::Update(double ts)
        {
            std::lock_guard<std::mutex> lock(Instance().processingMutex_);

            for (auto& event : Instance().events_)
            {
                for (auto& listener : Instance().listeners_)
                {
                    if (listener->ProcessEvent(*event))
                        break;
                }
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
                SANE_DEBUG("Event listener already added: {}", listener->Name());
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
                SANE_DEBUG("Event listener not found: {}", listener->Name());
            }
        }
    }
}