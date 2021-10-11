#include "sane/logging/log.hpp"

namespace Sane
{
	namespace Logging
	{
		const Sink GetSink() {
			auto llog = spdlog::create<Log>(std::string("llog"));
			spdlog::set_default_logger(llog);
			return std::dynamic_pointer_cast<Log>(llog->sinks()[0]);
		}
	}

	Log::Log()
	{
		Clear();
	}

	void Log::Render()
	{
		if (!ImGui::Begin("Console", &visible_))
		{
			ImGui::End();
			return;
		}

		if (ImGui::BeginPopup("Options"))
		{
			ImGui::Checkbox("Auto-scroll", &autoScroll_);
			ImGui::EndPopup();
		}

		ImGui::SameLine();
		bool clear = ImGui::Button("Clear");
		ImGui::SameLine();
		bool copy = ImGui::Button("Copy");
		ImGui::SameLine();
		filter_.Draw("Filter", -100.0f);

		ImGui::Separator();
		ImGui::BeginChild("scrolling", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);

		if (clear)
			Clear();

		if (copy)
			ImGui::LogToClipboard();

		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
		const char* buf = buffer_.begin();
		const char* buf_end = buffer_.end();
		if (filter_.IsActive())
		{
			for (int line_no = 0; line_no < lineOffsets_.Size; line_no++)
			{
				const char* line_start = buf + lineOffsets_[line_no];
				const char* line_end = (line_no + 1 < lineOffsets_.Size) ? (buf + lineOffsets_[line_no + 1] - 1) : buf_end;
				if (filter_.PassFilter(line_start, line_end))
					ImGui::TextUnformatted(line_start, line_end);
			}
		}
		else
		{
			ImGuiListClipper clipper;
			clipper.Begin(lineOffsets_.Size);
			while (clipper.Step())
			{
				for (int line_no = clipper.DisplayStart; line_no < clipper.DisplayEnd; line_no++)
				{
					const char* line_start = buf + lineOffsets_[line_no];
					const char* line_end = (line_no + 1 < lineOffsets_.Size) ? (buf + lineOffsets_[line_no + 1] - 1) : buf_end;

					switch (line_start[1])
					{
					case 'D':
						ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(.2f, .3f, .8f, 1.f));
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

		if (autoScroll_ && ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
			ImGui::SetScrollHereY(1.0f);

		ImGui::EndChild();
		ImGui::End();
	}

	namespace {
#pragma warning(disable : 4996)
		std::string format_time_point(std::chrono::system_clock::time_point point)
		{
			std::string out(29, '0');
			char* buf = &out[0];
			time_t now_c = std::chrono::system_clock::to_time_t(point);
			strftime(buf, 21, "%Y-%m-%d %H:%M:%S.", localtime(&now_c));
			sprintf_s(buf + 20, 10, "%09ld", size_t(point.time_since_epoch().count() % 1000000000));
			return out;
		}
	}

	void Log::sink_it_(const spdlog::details::log_msg& msg)
	{
		auto time = format_time_point(msg.time);

		switch (msg.level) {
		case spdlog::level::debug:
			AddLog("[DEBUG] %s :: %s\n", time.c_str(), std::string(msg.payload.data(), msg.payload.size()).c_str());
			break;
		case spdlog::level::warn:
			AddLog("[WARN]  %s :: %s\n", time.c_str(), std::string(msg.payload.data(), msg.payload.size()).c_str());
			break;
		case spdlog::level::err:
			AddLog("[FATAL] %s :: %s\n", time.c_str(), std::string(msg.payload.data(), msg.payload.size()).c_str());
			break;
		default:
			AddLog("[INFO]  %s :: %s\n", time.c_str(), std::string(msg.payload.data(), msg.payload.size()).c_str());
			break;
		}
	}

	void Log::flush_()
	{
	}

	void Log::Clear()
	{
		buffer_.clear();
		lineOffsets_.clear();
		lineOffsets_.push_back(0);
	}

	void Log::AddLog(const char* fmt, ...)
	{
		int old_size = buffer_.size();
		va_list args;
		va_start(args, fmt);
		buffer_.appendfv(fmt, args);
		va_end(args);
		for (int new_size = buffer_.size(); old_size < new_size; old_size++)
			if (buffer_[old_size] == '\n')
				lineOffsets_.push_back(old_size + 1);
	}


}