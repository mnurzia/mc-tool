#ifndef APP_COMPONENTS_HEXEDITOR_HEXEDITOR_HPP
#define APP_COMPONENTS_HEXEDITOR_HEXEDITOR_HPP

#include <cstdio>

#include <imgui/imgui_memory_editor.h>

#include "../WindowComponent.hpp"
#include "../../ImGuiEx.hpp"

// Code heavily taken from https://github.com/ocornut/imgui_club/blob/master/imgui_memory_editor/imgui_memory_editor.h

namespace App::Components::HexEditor {

	template <typename T>
	class HexEditor : public WindowComponent {
		std::vector<T>& data;
		MemoryEditor mem_edit;
		bool use_code_font = true;

	public:
		HexEditor(App* app, const std::string& window_name, std::vector<T>& data_) : WindowComponent(app, "Hex Editor: "+window_name, "hexed"), data(data_) {
			setWinFlags(ImGuiWindowFlags_MenuBar);
			setSize(ImVec2(680, 350));
			int8_t size = sizeof(T);
			if (size == 1) {
				mem_edit.PreviewDataType = mem_edit.DataType_S8;
			} else if (size == 2) {
				mem_edit.PreviewDataType = mem_edit.DataType_S16;
			} else if (size == 4) {
				mem_edit.PreviewDataType = mem_edit.DataType_S32;
			} else if (size == 8) {
				mem_edit.PreviewDataType = mem_edit.DataType_S64;
			}
			if (!std::is_signed<T>()) {
				mem_edit.PreviewDataType = (MemoryEditor::DataType)(((int8_t)mem_edit.PreviewDataType) + 1);
			}
			mem_edit.OptShowOptions = false;
			mem_edit.OptMidColsCount = sizeof(T);
		}

		void renderWin() {
			if (ImGui::BeginMenuBar()) {
				if (ImGui::BeginMenu("View")) {
					ImGui::MenuItem("Show ASCII", NULL, &mem_edit.OptShowAscii);
					ImGui::MenuItem("Show ASCII in Hex", NULL, &mem_edit.OptShowHexII);
					ImGui::MenuItem("Show Data Preview", NULL, &mem_edit.OptShowDataPreview);
					ImGui::MenuItem("Gray Out Zeroes", NULL, &mem_edit.OptGreyOutZeroes);
					ImGui::MenuItem("Uppercase Hex", NULL, &mem_edit.OptUpperCaseHex);
					ImGui::MenuItem("Use Code Font", NULL, &use_code_font);
					if (ImGui::BeginMenu("Address Digits")) {
						ImGui::SliderInt("##addrdigs", &mem_edit.OptAddrDigitsCount, 0, 16);
						ImGui::EndMenu();
					}
					if (ImGui::BeginMenu("Mid Cols")) {
						ImGui::SliderInt("##midcols", &mem_edit.OptMidColsCount, 0, 16);
						ImGui::EndMenu();
					}
					ImGui::EndMenu();
				}
				ImGui::EndMenuBar();
			}
			ImGui::Text("Offset:");
			ImGui::SameLine();
			ImGui::SetNextItemWidth(ImGui::CalcTextSize("FFFFFFFF     ").x);
			if (ImGui::InputText("##addr", mem_edit.AddrInputBuf, 32, ImGuiInputTextFlags_CharsHexadecimal | ImGuiInputTextFlags_EnterReturnsTrue)) {
				size_t goto_addr;
				if (sscanf(mem_edit.AddrInputBuf, "%zX", &goto_addr) == 1) {
					mem_edit.GotoAddrAndHighlight(goto_addr, 0);
					//mem_edit.GotoAddr = goto_addr;
					//mem_edit.HighlightMin = mem_edit.HighlightMax = (size_t)-1;
				}
			}
			if (use_code_font) {
				ImGuiEx::BeginCode();
			}
			mem_edit.DrawContents(data.data(), (data.size() * sizeof(T)));
			if (use_code_font) {
				ImGuiEx::EndCode();
			}
		}
	};

}

#endif