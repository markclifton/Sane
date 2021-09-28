#pragma once

#include <vector>
#include <mutex>

#include "imgui.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/base_sink.h"

#define SANE_DEBUG spdlog::debug
#define SANE_INFO spdlog::info
#define SANE_WARN spdlog::warn
#define SANE_ERROR spdlog::error

namespace Sane {
    class Log : public spdlog::sinks::base_sink<std::mutex>
    {
    public:
        Log();
        void Render();

    private:
        void sink_it_(const spdlog::details::log_msg& msg) override;
        void flush_() override;

        void Clear();
        void AddLog(const char* fmt, ...);

        bool autoScroll_{ true };
        ImGuiTextBuffer buffer_;
        ImGuiTextFilter filter_;
        ImVector<int> lineOffsets_;
        bool visible_{ true };
    };

    namespace Logging {
        typedef std::shared_ptr<Log> Sink;
        const Sink GetSink();
    }
}
