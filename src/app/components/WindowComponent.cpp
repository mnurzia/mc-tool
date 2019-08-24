#include "WindowComponent.hpp"

namespace App::Components {

	WindowComponent::WindowComponent(App* parent_, std::string win_name, std::string cls) : UIComponent(parent_, cls) {
		comp_win_name = this->windowName(win_name);
		size = ImVec2(0.0f, 0.0f);
	}

	std::string& WindowComponent::getWinName() {
		return comp_win_name;
	}

	void WindowComponent::setWinFlags(ImGuiWindowFlags in) {
		win_flags = in;
	}

	void WindowComponent::setClosed(bool* closed_) {
		closed = closed_;
	}

	void WindowComponent::setSize(ImVec2 in) {
		size = in;
	}

	void WindowComponent::render() {
		ImGuiViewport* cur_view = ImGui::GetMainViewport();
		glm::vec2 scr_sz = cur_view->Size;
		glm::vec2 scr_pos = cur_view->Pos;
		ImGui::SetNextWindowPos(scr_pos + scr_sz * 0.5f, ImGuiCond_Once, ImVec2(0.5f, 0.5f));
		ImGui::SetNextWindowSize(size, ImGuiCond_Once);
		bool* out = closed;
		if (out == NULL) {
			out = &should_quit;
		} else {
			if (!*closed) {
				return;
			}
		}
		if (ImGui::Begin(comp_win_name.c_str(), out, win_flags)) {
			this->renderWin();
		}
		ImGui::End();
		first = false;
	}

	void WindowComponent::renderWin() {

	}

	bool WindowComponent::isFirst() {
		return first;
	}

}