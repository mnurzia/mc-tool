#include "FontManager.hpp"

#include <imgui/imgui.h>
#include <imgui/misc/freetype/imgui_freetype.h>
#include <stb_image.h>

#include "../../ImGuiEx.hpp"
#include "../../Icons.hpp"

namespace App::Components::FontManager {

	FontManager::FontManager(App* app) : PersistentWindowComponent(app, "Font Manager", "fontmanager") {
		build_mode = FontBuildMode::FREETYPE;
		want_rebuild = true;
		font_multiply = 1.0f;
		font_padding = 1;
		font_flags = 0;
		ImGuiIO io = ImGui::GetIO();
		io.Fonts->AddFontFromFileTTF("/System/Library/Fonts/HelveticaNeue.ttc", scale * 16);
		reserveIconAtlas(16, APP_ICONS_MIN, APP_ICONS_MAX);
		ImGuiEx::LoadCodeFont(scale);
		logger->info("Loaded {} fonts.", io.Fonts->Fonts.size());
	}

	void FontManager::reserveIconAtlas(int isize, int min, int max) {
		ImGuiIO& io = ImGui::GetIO();
		rect_ids.clear();
		rect_ids.reserve(max - min);
		for (int i = 0; i < max - min; i++) {
			rect_ids.push_back(io.Fonts->AddCustomRectFontGlyph(io.Fonts->Fonts[0], min + i, 16, 16, 16 + 1));
		}
		logger->info("Loaded {} icons.", rect_ids.size());
	}

	void FontManager::loadIconAtlas(std::string fname, int isize, int min, int max, int atl_width, int atl_height) {
		ImGuiIO& io = ImGui::GetIO();
		unsigned char* tex_pixels = NULL;
		int tex_width;
		int tex_height;
		unsigned char* atlas_pixels = NULL;
		int atlas_width;
		int atlas_height;
		int atlas_bpp;
		atlas_pixels = stbi_load(fname.c_str(), &atlas_width, &atlas_height, &atlas_bpp, STBI_rgb_alpha);
		io.Fonts->GetTexDataAsRGBA32(&tex_pixels, &tex_width, &tex_height);
		for (int i = min - APP_ICONS_MIN; i < max - APP_ICONS_MIN; i++) {
			int rect_id = rect_ids[i];
			if (const ImFontAtlasCustomRect * rect = io.Fonts->GetCustomRectByIndex(rect_id)) {
				int atlas_y = (rect_id / atl_width);
				int atlas_x = (rect_id % atl_width);
				ImU32* atlas_ptr = ((ImU32*)atlas_pixels) + (isize * isize * atlas_y * atl_width) + (isize * atlas_x);
				ImU32* rect_ptr = ((ImU32*)tex_pixels) + rect->Y * tex_width + rect->X;
				for (int y = 0; y < rect->Height; y++) {
					for (int x = 0; x < rect->Width; x++) {
						*rect_ptr = *atlas_ptr;
						rect_ptr++;
						atlas_ptr++;
					}
					atlas_ptr += (atl_width - 1) * isize;
					rect_ptr += (tex_width - rect->Width);
				}
			}
		}
		free(atlas_pixels);
	}

	bool FontManager::updateRebuild() {
		if (!want_rebuild) {
			return false;
		}
		ImGuiIO& io = ImGui::GetIO();
		io.Fonts->TexGlyphPadding = font_padding;
		for (int n = 0; n < io.Fonts->ConfigData.Size; n++) {
			ImFontConfig* font_config = (ImFontConfig*)& io.Fonts->ConfigData[n];
			font_config->RasterizerMultiply = font_multiply;
			//font_config->RasterizerFlags = (build_mode == FontBuildMode::FREETYPE) ? font_flags : 0x00;
		}
		if (build_mode == FontBuildMode::FREETYPE) {
			ImGuiFreeType::BuildFontAtlas(io.Fonts, font_flags);
		} else if (build_mode == FontBuildMode::STB) {
			io.Fonts->Build();
		}
		loadIconAtlas("assets/icons_16.png", 16, APP_ICONS_MIN, APP_ICONS_MAX, APP_ICONS_ATLAS_WIDTH, APP_ICONS_ATLAS_HEIGHT);
		want_rebuild = false;
		logger->info("Rebuilt fonts and icons.");
		return true;
	}

	void FontManager::renderWin() {
		ImGui::ShowFontSelector("Fonts");
		want_rebuild |= ImGui::RadioButton("FreeType", (int*)& build_mode, int(FontBuildMode::FREETYPE));
		ImGui::SameLine();
		want_rebuild |= ImGui::RadioButton("Stb (Default)", (int*)& build_mode, int(FontBuildMode::STB));
		want_rebuild |= ImGui::DragFloat("Multiply", &font_multiply, 0.001f, 0.0f, 2.0f);
		want_rebuild |= ImGui::DragInt("Padding", &font_padding, 0.1f, 0, 16);
		if (build_mode == FontBuildMode::FREETYPE) {
			want_rebuild |= ImGui::CheckboxFlags("NoHinting", &font_flags, ImGuiFreeTypeBuilderFlags_NoHinting);
			want_rebuild |= ImGui::CheckboxFlags("NoAutoHint", &font_flags, ImGuiFreeTypeBuilderFlags_NoAutoHint);
			want_rebuild |= ImGui::CheckboxFlags("ForceAutoHint", &font_flags, ImGuiFreeTypeBuilderFlags_ForceAutoHint);
			want_rebuild |= ImGui::CheckboxFlags("LightHinting", &font_flags, ImGuiFreeTypeBuilderFlags_LightHinting);
			want_rebuild |= ImGui::CheckboxFlags("MonoHinting", &font_flags, ImGuiFreeTypeBuilderFlags_MonoHinting);
			want_rebuild |= ImGui::CheckboxFlags("Bold", &font_flags, ImGuiFreeTypeBuilderFlags_Bold);
			want_rebuild |= ImGui::CheckboxFlags("Oblique", &font_flags, ImGuiFreeTypeBuilderFlags_Oblique);
			want_rebuild |= ImGui::CheckboxFlags("Monochrome", &font_flags, ImGuiFreeTypeBuilderFlags_Monochrome);
		}
		want_rebuild |= ImGui::DragFloat("Scale", &scale, 0.1f, 0.5f, 8.0f);
	}

}
