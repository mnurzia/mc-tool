#ifndef APP_IMGUIEX_HPP
#define APP_IMGUIEX_HPP

#include <imgui/imgui.h>

namespace App::ImGuiEx {

	struct Icon {
		int8_t type = -1;
		union {
			struct _string {
				const char* start;
				const char* end;
			};
			_string string;
			struct _texture {
				ImTextureID tex_id;
				ImVec2 uva;
				ImVec2 uvb;
			};
			_texture texture;
		};
		ImU32 color = IM_COL32(255, 255, 255, 255);

		Icon(const char* start, const char* end = NULL, ImU32 color = IM_COL32(255, 255, 255, 255));

		Icon(ImTextureID tex_id, ImVec2 uva = ImVec2(0, 0), ImVec2 uvb = ImVec2(1, 1), ImU32 color = IM_COL32(255, 255, 255, 255));

		Icon(ImU32 color);

		Icon();
	};

	void AddIcon(Icon in);

	void AddIconLayer(Icon in);

	void MenuIcon(ImTextureID icon_texture, ImVec2 uva = ImVec2(0, 0), ImVec2 uvb = ImVec2(1, 1));

	void MenuIcon(const char* icon_string_start, const char* icon_string_end = NULL, ImU32 col = IM_COL32(255, 255, 255, 255));

	void MenuIcon(ImU32 col);

	void MenuIcon(struct Icon in);
	
	void MenuIconBack();

	void MenuSpacer();

	void LoadCodeFont(float scale = 1.0f);

	void BeginCode();

	void EndCode();

	void SelectableField(const char* string);

	void SelectableFieldCode(const char* string);

	void BeginPropertyEdit(const char* id);

	bool PropertyInt2(const char* name, int32_t* out);

	void PropertyStaticString(const char* name, const char* string_start);

	void EndPropertyEdit();

	void PrepareNextWindow(ImVec2 size = ImVec2(0, 0));

	void HelpMarker(const char* text);
}

#endif