#pragma once

#include <functional>
#include <sstream>

#include <imgui.h>

#define CreateDbgConsole(title)  Sane::Console console(title);
#define UpdateDbgConsole() PROCESS_LOGS(console);

namespace Sane
{
    class Console
    {
    public:
        Console(const char* title);

        void Clear();
        void Update(std::stringstream& ss);
        void AddLog(const char* fmt, ...);
        void Draw();

        operator std::function<void(std::stringstream& ss)>();

    private:
        const char* title;
        bool visible{ true };
        bool autoScroll{ true };
        ImGuiTextBuffer buffer;
        ImGuiTextFilter filter;
        ImVector<int> lineOffsets;
    };
}