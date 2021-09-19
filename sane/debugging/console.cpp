#include "sane/debugging/console.hpp"

#include "sane/debugging/logging.hpp"

namespace Sane
{
    Console::Console(const char* title)
        : title(title)
    {
        Clear();
        SANE_INFO("Successfully created debug console");
    }

    void Console::Clear()
    {
        buffer.clear();
        lineOffsets.clear();
        lineOffsets.push_back(0);
    }

    void Console::Update(std::stringstream& ss)
    {
        for (std::string line; std::getline(ss, line);) {
            AddLog("%s\n", line.c_str());
        }
        Draw();
    }

    void Console::AddLog(const char* fmt, ...)
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

    void Console::Draw()
    {
        if (!ImGui::Begin(title, &visible))
        {
            ImGui::End();
            return;
        }

        if (ImGui::BeginPopup("Options"))
        {
            ImGui::Checkbox("Auto-scroll", &autoScroll);
            ImGui::EndPopup();
        }

        ImGui::SameLine();
        bool clear = ImGui::Button("Clear");
        ImGui::SameLine();
        bool copy = ImGui::Button("Copy");
        ImGui::SameLine();
        filter.Draw("Filter", -100.0f);

        ImGui::Separator();
        ImGui::BeginChild("scrolling", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);

        if (clear)
            Clear();

        if (copy)
            ImGui::LogToClipboard();

        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
        const char* buf = buffer.begin();
        const char* buf_end = buffer.end();
        if (filter.IsActive())
        {
            for (int line_no = 0; line_no < lineOffsets.Size; line_no++)
            {
                const char* line_start = buf + lineOffsets[line_no];
                const char* line_end = (line_no + 1 < lineOffsets.Size) ? (buf + lineOffsets[line_no + 1] - 1) : buf_end;
                if (filter.PassFilter(line_start, line_end))
                    ImGui::TextUnformatted(line_start, line_end);
            }
        }
        else
        {
            ImGuiListClipper clipper;
            clipper.Begin(lineOffsets.Size);
            while (clipper.Step())
            {
                for (int line_no = clipper.DisplayStart; line_no < clipper.DisplayEnd; line_no++)
                {
                    const char* line_start = buf + lineOffsets[line_no];
                    const char* line_end = (line_no + 1 < lineOffsets.Size) ? (buf + lineOffsets[line_no + 1] - 1) : buf_end;

                    switch (line_start[1])
                    {
                    case 'D':
                        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(.2, .3, .8, 1));
                        break;
                    case 'I':
                        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0, 1, 0, 1));
                        break;
                    case 'W':
                        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1, 1, 0, 1));
                        break;
                    case 'F':
                        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1, 0, 0, 1));
                        break;
                    default:
                        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1, 1, 1, 1));
                    }

                    ImGui::TextUnformatted(line_start, line_end);
                    ImGui::PopStyleColor();
                }
            }
            clipper.End();
        }
        ImGui::PopStyleVar();

        if (autoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
            ImGui::SetScrollHereY(1.0f);

        ImGui::EndChild();
        ImGui::End();
    }

    Console::operator std::function<void(std::stringstream& ss)>() {
        return std::bind(&Console::Update, this, std::placeholders::_1);
    }
}