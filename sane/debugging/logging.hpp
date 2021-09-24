#pragma once

#include <vector>
#include <mutex>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/base_sink.h>
#include <imgui.h>

#define SANE_DEBUG spdlog::debug
#define SANE_INFO spdlog::info
#define SANE_WARN spdlog::warn
#define SANE_ERROR spdlog::error

namespace Sane {
    namespace Logging {
        class Log : public spdlog::sinks::base_sink<std::mutex>
        {
        public:
            void Render();
            Log()
            {
                Clear();
            }
        protected:
            void sink_it_(const spdlog::details::log_msg& msg) override;
            void flush_() override;

            void Clear()
            {
                buffer.clear();
                lineOffsets.clear();
                lineOffsets.push_back(0);
            }

            void AddLog(const char* fmt, ...)
            {
                int old_size = buffer.size();
                va_list args;
                va_start(args, fmt);
                buffer.appendfv(fmt, args);
                va_end(args);
                for (int new_size = buffer.size(); old_size < new_size; old_size++)
                    if (buffer[old_size] == '\n')
                        lineOffsets.push_back(old_size + 1);
            }

        private:
            bool autoScroll{ true };
            ImGuiTextBuffer buffer;
            ImGuiTextFilter filter;
            ImVector<int> lineOffsets;
            bool visible{ true };
        };

        typedef std::shared_ptr<Sane::Logging::Log> LogSink;
        const LogSink GetLogSink();
    }
}
