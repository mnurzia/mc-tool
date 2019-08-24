#include "ImGuiEx.hpp"
#include "Icons.hpp"

namespace App::ImGuiEx {

	static ImFont* code_font = NULL;

	Icon::Icon(const char* start, const char* end, ImU32 color_) {
		type = 0;
		string.start = start;
		string.end = end;
		color = color_;
	}

	Icon::Icon(ImTextureID icon_texture, ImVec2 uva, ImVec2 uvb, ImU32 color_) {
		type = 1;
		texture.tex_id = icon_texture;
		texture.uva = uva;
		texture.uvb = uvb;
		color = color_;
	}

	Icon::Icon(ImU32 color_) {
		type = 2;
		color = color_;
	}

	Icon::Icon() {
		type = -1;
		color = 0;
	}

	void AddIcon(Icon in) {
		float sz = ImGui::GetTextLineHeight();
		ImDrawList* draw_list = ImGui::GetWindowDrawList();
		ImVec2 curpos = ImGui::GetCursorScreenPos();
		switch (in.type) {
			case -1: {
				break;
			}
			case 0: {
				ImVec2 tsz = ImGui::CalcTextSize(in.string.start, in.string.end);
				ImVec2 tpos = ImVec2(
					curpos.x + ((sz / 2.0f) - ((tsz.x - 1.0f) / 2.0f)),
					curpos.y + 0
				);
				draw_list->AddText(tpos, in.color, in.string.start, in.string.end);
				break;
			}
			case 1: {
				draw_list->AddImage(in.texture.tex_id, curpos, ImVec2(curpos.x + sz, curpos.y + sz), in.texture.uva, in.texture.uvb, in.color);
				break;
			}
			case 2: {
				draw_list->AddRectFilled(curpos, ImVec2(curpos.x + sz, curpos.y + sz), in.color, 2.0f);
				break;
			}
		}
		ImGui::Dummy(ImVec2(sz, sz));
		ImGui::SameLine();
	}

	void AddIconLayer(Icon in) {
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() - ImGui::GetTextLineHeight() - ImGui::GetStyle().ItemSpacing.x);
		AddIcon(in);
	}

	void MenuIcon(ImTextureID icon_texture, ImVec2 uva, ImVec2 uvb) {
		float sz = ImGui::GetTextLineHeight();
		ImDrawList* draw_list = ImGui::GetWindowDrawList();
		ImVec2 curpos = ImGui::GetCursorScreenPos();
		draw_list->AddImage(icon_texture, curpos, ImVec2(curpos.x + sz, curpos.y + sz), uva, uvb);
		ImGui::Dummy(ImVec2(sz, sz));
		ImGui::SameLine();
	}

	void MenuIcon(const char* icon_string_start, const char* icon_string_end, ImU32 col) {
		float sz = ImGui::GetTextLineHeight();
		ImDrawList* draw_list = ImGui::GetWindowDrawList();
		glm::vec2 curpos = ImGui::GetCursorScreenPos();
		draw_list->AddText(curpos, col, icon_string_start, icon_string_end);
		ImGui::Dummy(ImVec2(sz, sz));
		ImGui::SameLine();
	}

	void MenuIcon(ImU32 col) {
		float sz = ImGui::GetTextLineHeight();
		ImDrawList* draw_list = ImGui::GetWindowDrawList();
		ImVec2 curpos = ImGui::GetCursorScreenPos();
		draw_list->AddRectFilled(curpos, ImVec2(curpos.x + sz, curpos.y + sz), col, 2.0f);
		ImGui::Dummy(ImVec2(sz, sz));
		ImGui::SameLine();
	}

	void MenuIconBack() {
		ImGui::SameLine(-1 * ImGui::GetTextLineHeight());
	}

	void MenuSpacer() {
		float sz = ImGui::GetTextLineHeight();
		ImGui::Dummy(ImVec2(sz, sz));
		ImGui::SameLine();
	}

	void LoadCodeFont(float scale) {
		code_font = ImGui::GetIO().Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\ltype.ttf", 15*scale);
	}

	void BeginCode() {
		ImGui::PushFont(code_font);
	}

	void EndCode() {
		ImGui::PopFont();
	}

	void SelectableField(const char* string) {
		ImGui::Text("%s", string);
		if (ImGui::BeginPopupContextItem(string)) {
			if (ImGui::MenuItem("Copy", "CTRL-C")) {
				ImGui::SetClipboardText(string);
			}
			ImGui::EndPopup();
		}
	}

	void SelectableFieldCode(const char* string) {
		BeginCode();
		ImGui::Text("%s", string);
		EndCode();
		if (ImGui::BeginPopupContextItem(string)) {
			if (ImGui::MenuItem("Copy", "CTRL-C")) {
				ImGui::SetClipboardText(string);
			}
			ImGui::EndPopup();
		}
	}

	void BeginPropertyEdit(const char* id) {
		ImGui::Columns(2, id);
	}

	bool PropertyInt2(const char* name, int32_t* out) {
		ImGui::Text(name);
		ImGui::NextColumn();
		ImGui::PushID(name);
		ImGui::PushItemWidth((ImGui::GetWindowSize().x) - ImGui::GetCursorPosX() - ImGui::GetStyle().FramePadding.x * 2);
		auto res = ImGui::InputInt2("", out);
		ImGui::PopItemWidth();
		ImGui::PopID();
		ImGui::NextColumn();
		return res;
	}

	void PropertyStaticString(const char* name, const char* string_start) {
		ImGui::Text(name);
		ImGui::NextColumn();
		SelectableFieldCode(string_start);
		ImGui::NextColumn();
	}

	void EndPropertyEdit() {
		ImGui::Columns(1);
	}

	void PrepareNextWindow(ImVec2 size) {
		ImGui::SetNextWindowPos(
			glm::vec2(ImGui::GetIO().DisplaySize) / 2.0f +
			glm::vec2(ImGui::GetMainViewport()->Pos),
			ImGuiCond_Once,
			ImVec2(0.5, 0.5)
		);
		ImGui::SetNextWindowSize(size, ImGuiCond_Once);
	}

	void HelpMarker(const char* text) {
		ImGui::TextDisabled("?");
		if (ImGui::IsItemHovered()) {
			ImGui::BeginTooltip();
			ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
			ImGui::TextUnformatted(text);
			ImGui::PopTextWrapPos();
			ImGui::EndTooltip();
		}
	}
}