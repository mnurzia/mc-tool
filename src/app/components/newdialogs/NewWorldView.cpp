#include "NewWorldView.hpp"

#include <random>
#include <ctime>

#include <glm/vec2.hpp>
#include <imgui/imgui.h>

#include "../../App.hpp"
#include "../worldview/WorldView.hpp"

namespace App::Components::NewDialogs {

	NewWorldView::NewWorldView(App* parent_) : UIComponent(parent_, "newdialogs.newworldview") {
		win_name = this->windowName("New World View");
	}

	int64_t NewWorldView::parseSeed(char* in, int* type) {
		bool is_int = true;
		bool neg = false;
		int64_t accum = 0;
		char* ptr = in;
		if (*ptr == '\0') {
			*type = 2;
			return 0;
		}
		if (*ptr == '-') {
			ptr++;
			neg = true;
		}
		while (*ptr) {
			if (!(*ptr >= '0' && *ptr <= '9')) {
				is_int = false;
				break;
			} else {
				accum *= 10;
				accum += *ptr - '0';
			}
			ptr++;
		}
		if (is_int) {
			if (neg) {
				accum *= -1;
			}
			*type = 0;
			return accum;
		} else {
			int32_t out = 0;
			ptr = in;
			while (*ptr) {
				out *= 31;
				out += *ptr;
				ptr++;
			}
			*type = 1;
			return int64_t(out);
		}
	}

	void NewWorldView::render() {
		ImGui::SetNextWindowPos(
			glm::vec2(ImGui::GetIO().DisplaySize) / 2.0f + glm::vec2(ImGui::GetMainViewport()->Pos),
			ImGuiCond_Once,
			ImVec2(0.5f, 0.5f)
		);
		if (ImGui::Begin(win_name.c_str(), &should_quit, ImGuiWindowFlags_AlwaysAutoResize)) {
			ImGui::Text("Enter Seed:");
			if (ImGui::InputText("##seed", seed_input, sizeof(seed_input) / sizeof(char), ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_EnterReturnsTrue)) {
				should_proceed = true;
				quit();
			}
			int type = 0;
			seed = parseSeed(seed_input, &type);
			if (type == 0) {
				ImGui::Text("Integer Seed: %lli", seed);
			} else if (type == 1) {
				ImGui::Text("String Seed: %li", seed);
			} else if (type == 2) {
				ImGui::Text("Random Seed");
			}
			if (ImGui::Button("Cancel")) {
				quit();
			}
			ImGui::SameLine();
			if (ImGui::Button("OK")) {
				should_proceed = true;
				if (type == 2) {
					std::srand((unsigned int)std::time(nullptr));
					for (int i = 0; i < 8; i++) {
						seed += (std::rand() & 0xFF);
						seed <<= 8;
					}
				}
				quit();
			}
		}
		ImGui::End();
		if (canQuit()) {
			if (should_proceed) {
				getParent()->addComponent(
					std::unique_ptr<UIComponent>(new WorldView::WorldView(getParent(), seed))
				);
			}
		}
	}

}