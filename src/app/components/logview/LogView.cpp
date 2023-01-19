#include "LogView.hpp"

#include <ctime>

#include <chrono>
#include <vector>

#include "../../../util/Color.hpp"

#include "Sink.hpp"

namespace App::Components::LogView {

	LogView::LogView(App* parent_) : PersistentWindowComponent(parent_, "Log Viewer", "logview") {
		setSize(ImVec2(720, 300));
	}

	void LogView::renderWin() {
		spdlog::sink_ptr t_sink = getParent()->getLogger()->sinks()[1];
		std::shared_ptr<Sink> sink = std::static_pointer_cast<Sink>(t_sink);
		std::vector<Line>& msgs = sink->getMessages();
		ImGui::BeginChild("##ScrollingRegion", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);
		ImGui::Columns(4, "log_columns");
		ImGui::Text("Time"); ImGui::NextColumn();
		ImGui::Text("Level"); ImGui::NextColumn();
		ImGui::Text("Logger"); ImGui::NextColumn();
		ImGui::Text("Message"); ImGui::NextColumn();
		if (isFirst()) {
			ImGui::SetColumnWidth(0, 150);
			ImGui::SetColumnWidth(1, 60);
			ImGui::SetColumnWidth(2, 100);
			ImGui::SetColumnWidth(3, 400);
		}
		ImGui::Separator();
		for (auto line : msgs) {
			ImGui::Text(line.time_fmt.c_str()); ImGui::NextColumn();
			spdlog::string_view_t names[] = SPDLOG_LEVEL_NAMES;
			Util::Color out_color = Util::Color(255, 255, 255);
			switch (line.level) {
				case spdlog::level::trace:
					out_color = Util::Color(192, 192, 192);
					break;
				case spdlog::level::debug:
					out_color = Util::Color(  0, 192, 192);
					break;
				case spdlog::level::info:
					out_color = Util::Color(  0, 192,   0);
					break;
				case spdlog::level::warn:
					out_color = Util::Color(192, 192,   0);
					break;
				case spdlog::level::err:
					out_color = Util::Color(192,   0,   0);
					break;
				case spdlog::level::critical:
					out_color = Util::Color(192,   0, 192);
					break;
			}
			ImGui::TextColored(out_color, names[line.level].data()); ImGui::NextColumn();
			ImGui::Text(line.logger_name.c_str()); ImGui::NextColumn();
			ImGui::Text(line.content.c_str()); ImGui::NextColumn();
		}
		ImGui::Columns(1);
		ImGui::EndChild();
	}

}
