#include "ThreadStatus.hpp"

#include <imgui/imgui.h>

#include "../../App.hpp"

namespace App::Components::ThreadStatus {

	ThreadStatus::ThreadStatus(App* parent_) : UIComponent(parent_, "threadstatus") {

	}

	void ThreadStatus::render() {
		decay -= ImGui::GetIO().DeltaTime / 3;
		if (decay < 0) {
			decay = 0;
		}
		ImGuiIO& io = ImGui::GetIO();
		ImGuiViewport* main = ImGui::GetMainViewport();
		ImGui::SetNextWindowViewport(main->ID);
		glm::vec2 pos = glm::vec2(io.DisplaySize);
		float height = ImGui::GetFontSize() + ImGui::GetStyle().DisplaySafeAreaPadding.y + 2;
		pos.x = 0;
		pos.y -= height;
		pos += glm::vec2(main->Pos);
		ImGui::SetNextWindowPos(pos);
		glm::vec2 size = io.DisplaySize;
		size.y = height;
		ImGui::SetNextWindowSize(size);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(10.0f, 2.0f));
		if (
			ImGui::Begin(
				"Thread Queue Status",
				&should_quit,
				ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoDecoration |
				ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings |
				ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav |
				ImGuiWindowFlags_NoDocking
			)
			) {
			ImDrawList* draw_list = ImGui::GetWindowDrawList();
			glm::vec2 crmin = draw_list->GetClipRectMin();
			glm::vec2 crmax = draw_list->GetClipRectMax();
			draw_list->PopClipRect();
			draw_list->AddRectFilled(pos, pos + size, ImColor(ImGui::GetStyle().Colors[ImGuiCol_MenuBarBg]));
			auto& task_queue = getParent()->getDispatchQueue();
			auto comp_queue = task_queue.getComposition();
			int queue_queued = std::get<1>(comp_queue);
			int queue_running = std::get<2>(comp_queue);
			if (queue_running) {
				if (decay == 0.0f) {
					prog = 0.0f;
					prog_next = 0.0f;
				}
				decay = 1.0f;
				auto groups = task_queue.getGroups();
				for (auto grp : groups) {
					auto comp = grp.second->getComposition();
					int init = std::get<0>(comp);
					int queued = std::get<1>(comp);
					int running = std::get<2>(comp);
					int upper = grp.second->getUpper();
					int processed = grp.second->getProcessed();
					float rat = 1.0f;
					if (upper != 0) {
						rat = float(processed) / float(upper);
					}
					prog_next = rat;
					prog = prog + (prog_next - prog) * 0.8f * (io.DeltaTime * 4.0f);
					draw_list->AddRectFilled(pos, pos + size * glm::vec2(prog, 1), ImColor(glm::vec4(ImGui::GetStyle().Colors[ImGuiCol_ButtonHovered]) * decay));
					ImGui::Text("%s<%i/%i/%i>: %.2f%%", grp.second->getName().c_str(), init, queued, running, double(double(rat) * 100));
					break;
				}
			} else {
				prog = 1.0f;
				draw_list->AddRectFilled(pos, pos + size * glm::vec2(prog, 1), ImColor(glm::vec4(ImGui::GetStyle().Colors[ImGuiCol_ButtonHovered]) * decay));
				ImGui::Text("Idle");
			}
			char fps_buf[32];
			snprintf(fps_buf, 32, "%.2f FPS (%.3f ms)", io.Framerate, 1000 / io.Framerate);
			ImGui::SameLine(ImGui::GetWindowWidth() - ImGui::CalcTextSize(fps_buf).x - ImGui::GetStyle().WindowPadding.x);
			ImGui::Text(fps_buf);
			draw_list->PushClipRect(crmin, crmax);
		}
		ImGui::End();
		ImGui::PopStyleVar();
		ImGui::PopStyleVar();
	}

}