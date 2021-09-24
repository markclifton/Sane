#include <sane/debugging/logging.hpp>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/base_sink.h>

namespace Sane
{
	namespace Logging
	{
		const LogSink GetLogSink() {
			auto llog = spdlog::create<Log>(std::string("llog"));
			spdlog::set_default_logger(llog);
			return std::dynamic_pointer_cast<Log>(llog->sinks()[0]);
		}

		void Log::Render()
		{
			if (!ImGui::Begin("Log", &visible))
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

		void Log::sink_it_(const spdlog::details::log_msg& msg)
		{
			switch (msg.level) {
			case spdlog::level::debug:
				AddLog("[DEBUG] :: %s\n", std::string(msg.payload.data(), msg.payload.size()).c_str());
				break;
			case spdlog::level::warn:
				AddLog("[WARN]  :: %s\n", std::string(msg.payload.data(), msg.payload.size()).c_str());
				break;
			case spdlog::level::err:
				AddLog("[FATAL] :: %s\n", std::string(msg.payload.data(), msg.payload.size()).c_str());
				break;
			default:
				AddLog("[INFO]  :: %s\n", std::string(msg.payload.data(), msg.payload.size()).c_str());
				break;
			}
		}

		void Log::flush_()
		{
		}

	}
}