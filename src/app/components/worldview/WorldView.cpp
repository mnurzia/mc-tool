#include "WorldView.hpp"

#include <cmath>

#include <stb_image.h>

#include "../../../worldgen/Biome.hpp"
#include "../../ImGuiEx.hpp"
#include "../../Icons.hpp"

namespace App::Components::WorldView {

	float feature_sizes[int(WorldGen::Feature::Type::COUNT)] = {
		1.0f,
		1.0f,
		1.0f,
		1.0f,
		1.0f,
		1.0f,
		1.0f,
		1.0f,
		1.0f,
		1.0f,
		1.0f,
		1.0f,
		1.0f,
		1.0f,
		1.0f
	};

	const int feature_icon_idx[int(WorldGen::Feature::Type::COUNT)] = {
		 5,  0,  1,  2,  3,  4,  6,  7,
		 8,  9, 10, 11, 13, 12, 14
	};

	ImTextureID feature_texture = 0;

	WorldView::WorldView(App* app_, int64_t seed_) : UIComponent(app_, "worldview"), fmgr(*app_, seed_) {
		seed = seed_;
		logger->info("Initializing world view, seed {}", seed);
		win_viewer_name = windowName("World Viewer: " + std::to_string(seed_));
		win_pos_name = windowName("Position");
		win_debug_name = windowName("WorldView Debug Info");
		for (int i = 0; i < int(WorldGen::Feature::Type::COUNT); i++) {
			draw_features[i] = true;
		}
		if (!feature_texture) {
			int x, y, bpp;
			unsigned char* feature_data = stbi_load("assets/feature_icons.png", &x, &y, &bpp, STBI_rgb_alpha);
			feature_texture = getParent()->bindTexture(feature_texture, (int32_t*)feature_data, x, y);
		}
	}

	glm::vec2 WorldView::toWorld(glm::vec2 in) {
		glm::vec2 size = ImGui::GetWindowSize();
		glm::vec2 win = ImGui::GetWindowPos();
		in -= win;
		in -= size * 0.5f;
		in = in * zoom_ratio;
		return in + offset;
	}

	glm::vec2 WorldView::toScreen(glm::vec2 in) {
		glm::vec2 size = ImGui::GetWindowSize();
		glm::vec2 win = ImGui::GetWindowPos();
		in -= offset;
		in = in / zoom_ratio;
		in += size * 0.5f;
		return in + win;
	}

	bool WorldView::quitLogic() {
		quitting = !frame_quit;
		if (quitting) {
			fmgr.setQuit(true);
			fmgr.tryQuit();
			if (fmgr.getQuit()) {
				fmgr.destroy();
				really_quit = true;
			}
		}
		if (really_quit) {
			quit();
			return false;
		}
		return true;
	}

	void WorldView::drawGrid() {
		ImDrawList* draw_list = ImGui::GetWindowDrawList();
		// Find label size that fits best into 128 pixels
		int32_t texture_size = fmgr.getTextureSize();
		float label_size = 16;
		float sector_size = 16 / zoom_ratio;
		while (sector_size < 128) {
			sector_size *= 2;
			label_size *= 2;
		}
		glm::vec2 tls = ImGui::GetWindowPos();
		glm::vec2 brs = tls + glm::vec2(ImGui::GetWindowSize());
		glm::vec2 tl  = toWorld(tls);
		glm::vec2 br  = toWorld(brs);
		int32_t begin_x = int32_t(floor(tl.x / label_size) * label_size);
		int32_t begin_z = int32_t(floor(tl.y / label_size) * label_size);
		int32_t end_x   = int32_t( ceil(br.x / label_size) * label_size);
		int32_t end_z   = int32_t( ceil(br.y / label_size) * label_size);
		if (draw_grid) {
			for (int32_t start_x = begin_x; start_x < end_x; start_x += int32_t(label_size)) {
				draw_list->AddLine(
					toScreen(glm::vec2(start_x, begin_z)),
					toScreen(glm::vec2(start_x, end_z)),
					IM_COL32(255, 255, 255, 255)
				);
			}
			for (int32_t start_z = begin_z; start_z < end_z; start_z += int32_t(label_size)) {
				draw_list->AddLine(
					toScreen(glm::vec2(begin_x, start_z)),
					toScreen(glm::vec2(end_x, start_z)),
					IM_COL32(255, 255, 255, 255)
				);
			}
		}
		if (draw_grid_labels) {
			glm::vec2 padding = glm::vec2(4, 3);
			glm::vec2 offset = glm::vec2(4, 4);
			for (int32_t start_z = begin_z; start_z < end_z; start_z += int32_t(label_size)) {
				for (int32_t start_x = begin_x; start_x < end_x; start_x += int32_t(label_size)) {
					char label_text[64];
					snprintf(label_text, 64, "(%i, %i)", start_x, start_z);
					glm::vec2 og = toScreen(glm::vec2(start_x, start_z));
					glm::vec2 xy0 = og + offset;
					glm::vec2 xy1 = xy0 + padding * 2.0f + glm::vec2(ImGui::CalcTextSize(label_text));
					glm::vec2 xy = xy0 + padding;
					draw_list->AddRectFilled(xy0, xy1, IM_COL32(0, 0, 0, 128), 2);
					draw_list->AddText(xy, IM_COL32(255, 255, 255, 255), label_text);
				}
			}
		}
	}

	void WorldView::drawInfo() {
		ImGuiIO io = ImGui::GetIO();
		glm::vec2 savedpos = ImGui::GetCursorPos();
		ImGui::Text("(%i, %i) @ %.2f%%", int(mouse.x), int(mouse.y), (double(1.0f) / double(zoom_ratio)) * 100);
		if (selected_feature.getType() != WorldGen::Feature::Type::NONE) {
			ImGui::SetCursorPos(savedpos);
			char buf[128];
			snprintf(buf, 128, "Feature: %s (%i, %i)", selected_feature.getName().c_str(), selected_feature.getX(), selected_feature.getZ());
			ImGui::SetCursorPosX(ImGui::GetWindowWidth() - ImGui::CalcTextSize(buf).x - ImGui::GetStyle().WindowPadding.x - savedpos.x);
			ImGui::Text(buf);
		} else if (current_biome != WorldGen::Biome::ID::NONE) {
			ImGui::SetCursorPos(savedpos);
			char buf[128];
			snprintf(buf, 128, "Biome: %s", WorldGen::Biome::biomes[int32_t(current_biome)].minecraft_id.c_str());
			ImGui::SetCursorPosX(ImGui::GetWindowWidth() - ImGui::CalcTextSize(buf).x - ImGui::GetStyle().WindowPadding.x - savedpos.x);
			ImGui::Text(buf);
		}
	}

	void WorldView::drawFragments() {
		rendered_frags = 0;
		rendered_feats = 0;
		ImGuiIO io = ImGui::GetIO();
		if (ImGui::IsWindowHovered(ImGuiHoveredFlags_ChildWindows)) {
			mouse = toWorld(glm::vec2(io.MousePos));
		}
		ImDrawList* draw_list = ImGui::GetWindowDrawList();
		int32_t texture_size = fmgr.getTextureSize();
		glm::vec2 tls = ImGui::GetWindowPos();
		glm::vec2 brs = tls + glm::vec2(ImGui::GetWindowSize());
		glm::vec2 tl = toWorld(tls);
		glm::vec2 br = toWorld(brs);
		int32_t begin_x = int32_t(floor(tl.x / texture_size) * texture_size);
		int32_t begin_z = int32_t(floor(tl.y / texture_size) * texture_size);
		int32_t end_x = int32_t(ceil(br.x / texture_size) * texture_size);
		int32_t end_z = int32_t(ceil(br.y / texture_size) * texture_size);
		current_biome = WorldGen::Biome::ID::NONE;
		std::vector<Fragment*> this_render;
		for (; begin_z < end_z; begin_z += texture_size) {
			int32_t start_x = begin_x;
			int32_t start_z = begin_z;
			for (; start_x < end_x; start_x += texture_size) {
				glm::vec2 xy0 = toScreen(glm::vec2(float(start_x), float(start_z)));
				glm::vec2 xy1 = toScreen(glm::vec2(float(start_x + texture_size), float(start_z + texture_size)));
				Fragment* frag = fmgr.getFragment(start_x, start_z);
				if (frag != NULL) {
					if (float(start_x) <= mouse.x && float(start_z) <= mouse.y) {
						if (float(start_x + texture_size) >= mouse.x && float(start_z + texture_size) >= mouse.y) {
							glm::vec2 data_off = mouse;
							data_off -= glm::vec2(float(start_x), float(start_z));
							current_biome = WorldGen::Biome::ID(frag->getData()[int(data_off.y) * texture_size + int(data_off.x)]);
						}
					}
				}
				if (frag) {
					if (frag->getOpacity() < 1.0f) {
						frag->setOpacity(frag->getOpacity() + ImGui::GetIO().DeltaTime * 3.5f);
					}
					if (frag->getOpacity() > 1.0f) {
						frag->setOpacity(1.0f);
					}
					draw_list->AddDrawCmd();
					draw_list->PrimReserve(6, 4);
					draw_list->PrimRectUV(xy0, xy1, ImVec2(0, 0), ImVec2(1, 1), IM_COL32(255, 255, 255, 255 * frag->getOpacity()));
					draw_list->CmdBuffer.back().UserCallback = &Fragment::render;
					draw_list->CmdBuffer.back().UserCallbackData = frag;
					draw_list->AddCallback(ImDrawCallback_ResetRenderState, NULL);
					frag->setViewport(ImGui::GetWindowViewport());
					if (draw_slime_chunks) {
						draw_list->AddImage(frag->getSlimeTexture(), xy0, xy1, ImVec2(0, 0), ImVec2(1, 1), IM_COL32(255, 0, 255, 128));
					}
					this_render.push_back(frag);
					rendered_frags++;
				}
				else {
					draw_list->AddRectFilled(xy0, xy1, ImColor(0));
				}
			}
		}
		WorldGen::Feature* select_candidate = NULL;
		for (auto frag : this_render) {
			for (int i = 0; i < frag->getFeatures().size(); i++) {
				WorldGen::Feature* cur = &(frag->getFeatures()[i]);
				int feature_type = int(cur->getType());
				if (draw_features[feature_type]) {
					float scale = feature_sizes[feature_type];
					if (*cur == selected_feature) {
						scale *= 2.0f;
					}
					glm::vec2 org = toScreen(
						glm::vec2(float(cur->getX()), float(cur->getZ()))
					);
					org.x = round(org.x);
					org.y = round(org.y);
					glm::vec2 xy0 = org - glm::vec2(9, 9) * scale;
					glm::vec2 xy1 = xy0 + glm::vec2(18, 18) * scale;
					glm::vec2 uva = glm::vec2(
						float(feature_icon_idx[feature_type]) / float(WorldGen::Feature::Type::COUNT),
						0.0f
					);
					glm::vec2 uvb = glm::vec2(
						float(feature_icon_idx[feature_type] + 1) / float(WorldGen::Feature::Type::COUNT),
						1.0f
					);
					draw_list->AddImage(
						feature_texture,
						xy0,
						xy1,
						uva,
						uvb
					);
					rendered_feats++;
					if (io.MouseClicked[0]) {
						glm::vec2 mpos = io.MouseClickedPos[0];
						glm::vec2 dist = mpos - org;
						if (abs(dist.x) + abs(dist.y) < 8.0f) {
							select_candidate = cur;
						}
					}
					/*draw_list->AddText(
						toScreen(
							glm::vec2(float(cur->getX()), float(cur->getZ()))
						),
						IM_COL32(255, 255, 255, 255),
						cur->getName().c_str()
					);*/
				}
			}
		}
		if (select_candidate != NULL) {
			selected_feature = *select_candidate;
		} else {
			if (io.MouseClicked[0]) {
				selected_feature = WorldGen::Feature(0, 0, WorldGen::Feature::Type::NONE);
			}
		}
	}

	void WorldView::drawXHair() {
		ImDrawList* draw_list = ImGui::GetWindowDrawList();
		glm::vec2 center_pos = glm::vec2(ImGui::GetWindowPos()) + glm::vec2(ImGui::GetWindowSize()) / 2.0f;
		float outer = 15;
		float inner = 5;
		draw_list->AddLine(center_pos + glm::vec2( outer,  outer), center_pos + glm::vec2( inner,  inner), IM_COL32(255, 255, 255, 255), 2.0f);
		draw_list->AddLine(center_pos + glm::vec2( outer, -outer), center_pos + glm::vec2( inner, -inner), IM_COL32(255, 255, 255, 255), 2.0f);
		draw_list->AddLine(center_pos + glm::vec2(-outer,  outer), center_pos + glm::vec2(-inner,  inner), IM_COL32(255, 255, 255, 255), 2.0f);
		draw_list->AddLine(center_pos + glm::vec2(-outer, -outer), center_pos + glm::vec2(-inner, -inner), IM_COL32(255, 255, 255, 255), 2.0f);
	}

	void WorldView::processInputs() {
		ImGuiIO io = ImGui::GetIO();
		if (ImGui::IsWindowHovered()) {
			if (!lock_offset) {
				if (ImGui::IsMouseClicked(0)) {
					click = offset;
					vel = glm::vec2();
					drag_in = true;
				}
				zoom_want += io.MouseWheel * -0.1f;
			}
		}
		if (ImGui::IsMouseDragging(0) && drag_in) {
			if (!lock_offset) {
				ImVec2 delt = ImGui::GetMouseDragDelta();
				offset = click + glm::vec2(delt) * zoom_ratio * -1.0f;
				vel = glm::vec2(io.MouseDelta) * zoom_ratio * -1.0f;
			}
		}
		if (ImGui::IsMouseReleased(0)) {
			drag_in = false;
		}
		if (!ImGui::IsMouseClicked(0) && !ImGui::IsMouseDragging(0)) {
			if (!lock_offset) {
				offset += vel;
				vel *= friction;
			}
		}
		if (zoom_want > 10.0f) {
			zoom_want = 10.0f;
		}
		if (zoom_want < 0.1f) {
			zoom_want = 0.1f;
		}
		zoom_ratio = zoom_ratio + (zoom_want - zoom_ratio) * 0.3f;
	}

	void WorldView::drawMenuBar() {
		if (ImGui::BeginMenuBar()) {
			if (ImGui::BeginMenu("View")) {
				ImGuiEx::MenuSpacer();
				ImGui::MenuItem("Grid", NULL, &draw_grid);
				ImGuiEx::MenuIcon(ICON_PASTEL_TAG);
				ImGui::MenuItem("Grid Labels", NULL, &draw_grid_labels);
				ImGuiEx::MenuSpacer();
				ImGui::MenuItem("Slime Chunks", NULL, &draw_slime_chunks);
				ImGuiEx::MenuIcon(ICON_PASTEL_STAR);
				if (ImGui::BeginMenu("Features")) {
					ImGuiEx::MenuIcon(ICON_PASTEL_ADD);
					if (ImGui::MenuItem("All On")) {
						for (int i = 0; i < int(WorldGen::Feature::Type::COUNT); i++) {
							draw_features[i] = true;
						}
					}
					ImGuiEx::MenuIcon(ICON_PASTEL_DELETE);
					if (ImGui::MenuItem("All Off")) {
						for (int i = 0; i < int(WorldGen::Feature::Type::COUNT); i++) {
							draw_features[i] = false;
						}
					}
					ImGui::Separator();
					for (int i = 0; i < int(WorldGen::Feature::Type::COUNT); i++) {
						int x = feature_icon_idx[i];
						ImGuiEx::MenuIcon(
							feature_texture,
							ImVec2(float(x) / float(WorldGen::Feature::Type::COUNT), 0),
							ImVec2(float(x + 1) / float(WorldGen::Feature::Type::COUNT), 1)
						);
						ImGui::MenuItem(WorldGen::feature_names[i].c_str(), NULL, &(draw_features[i]));
					}
					ImGui::EndMenu();
				}
				ImGuiEx::MenuSpacer();
				if (ImGui::BeginMenu("Biomes")) {
					ImGuiEx::MenuIcon(ICON_PASTEL_ADD);
					if (ImGui::MenuItem("All On")) {
						for (int i = 0; i < 256; i++) {
							fmgr.setPalette(i);
						}
					}
					ImGuiEx::MenuIcon(ICON_PASTEL_DELETE);
					if (ImGui::MenuItem("All Off")) {
						for (int i = 0; i < 256; i++) {
							fmgr.unsetPalette(i);
						}
					}
					ImGui::Separator();
					for (int i = 0; i < 256; i++) {
						if (WorldGen::Biome::exists((WorldGen::Biome::ID)i)) {
							std::string& biome_name = WorldGen::Biome::biomes[i].minecraft_id;
							bool toggle = true;
							if (!fmgr.getPaletteSet(i)) {
								toggle = false;
							}
							uint32_t color = fmgr.getPalette(i);
							ImGuiEx::MenuIcon(color);
							if (ImGui::MenuItem(biome_name.c_str(), NULL, &toggle)) {
								if (toggle) {
									fmgr.setPalette(i);
								} else {
									fmgr.unsetPalette(i);
								}
							}
						}
					}
					ImGui::EndMenu();
				}
				ImGuiEx::MenuSpacer();
				if (ImGui::BeginMenu("Palette")) {
					bool realistic_ocean = fmgr.getPaletteMode(0);
					bool realistic_land = fmgr.getPaletteMode(1);
					if (ImGui::MenuItem("Realistic Ocean", NULL, &realistic_ocean)) {
						fmgr.buildPalette(realistic_ocean, realistic_land);
					}
					if (ImGui::MenuItem("Realistic Land", NULL, &realistic_land)) {
						fmgr.buildPalette(realistic_ocean, realistic_land);
					}
					ImGui::EndMenu();
				}
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Window")) {
				ImGui::MenuItem("Position", NULL, &win_pos_open);
				ImGui::MenuItem("Biome Info");
				ImGui::MenuItem("Debug Info", NULL, &win_debug_open);
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}
	}

	void WorldView::drawPosInfo() {
		ImGuiIO io = ImGui::GetIO();
		lock_offset = false;
		int32_t block_pos[2] = { 0, 0 };
		block_pos[0] = int(offset.x);
		block_pos[1] = int(offset.y);
		int32_t chunk_pos[2] = { block_pos[0] >> 4, block_pos[1] >> 4 };
		int32_t region_pos[2] = { chunk_pos[0] >> 5, chunk_pos[1] >> 5 };
		int32_t region_local_pos[2] = { chunk_pos[0] & 0x1F, chunk_pos[1] & 0x1F };
		ImGuiEx::BeginPropertyEdit("pos_info");
		if (ImGuiEx::PropertyInt2("Block", block_pos)) {
			offset.x = float(block_pos[0]);
			offset.y = float(block_pos[1]);
		}
		lock_offset |= ImGui::IsItemActive();
		if (ImGuiEx::PropertyInt2("Chunk", chunk_pos)) {
			offset.x = float(chunk_pos[0] << 4);
			offset.y = float(chunk_pos[1] << 4);
		}
		lock_offset |= ImGui::IsItemActive();
		if (ImGuiEx::PropertyInt2("Region", region_pos)) {
			offset.x = float(region_pos[0] << 9);
			offset.y = float(region_pos[1] << 9);
		}
		lock_offset |= ImGui::IsItemActive();
		if (ImGuiEx::PropertyInt2("Region Local", region_local_pos)) {
			offset.x = float((region_pos[0] << 9) + (region_local_pos[0] << 4));
			offset.y = float((region_pos[1] << 9) + (region_local_pos[1] << 4));
		}
		lock_offset |= ImGui::IsItemActive();
		char region[64];
		snprintf(region, 64, "r.%i.%i.mca", region_pos[0], region_pos[1]);
		ImGuiEx::PropertyStaticString("Region File", region);
		snprintf(region, 64, "%lli", seed);
		ImGuiEx::PropertyStaticString("Seed", region);
		ImGuiEx::EndPropertyEdit();
		ImGui::SameLine();
		ImGuiEx::HelpMarker("Right click 'region' and 'seed' fields to copy.");
	}

	void WorldView::drawDebug() {
		ImGui::Text("Rendered Fragments: %i\n", rendered_frags);
		ImGui::Text("Rendered Features: %i\n", rendered_feats);
	}

	void WorldView::render() {
		if (!quitLogic()) {
			return;
		}
		ImU32 dock_id = 0;
		ImGui::SetNextWindowPos(
			glm::vec2(ImGui::GetIO().DisplaySize) / 2.0f +
			glm::vec2(ImGui::GetMainViewport()->Pos),
			ImGuiCond_Once,
			ImVec2(0.5, 0.5)
		);
		if (ImGui::Begin(win_viewer_name.c_str(), &frame_quit, ImVec2(640, 480), -1, ImGuiWindowFlags_MenuBar)) {
			drawMenuBar();
			drawInfo();
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
			ImGui::BeginChild("ScrollingView", ImVec2(0, 0), true, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoDocking);
			drawFragments();
			drawGrid();
			if (lock_offset) {
				drawXHair();
			}
			processInputs();
			ImGui::EndChild();
			ImGui::PopStyleVar();
			fmgr.collectGarbage();
			fmgr.nextFrame();
		}
		ImGui::End();
		if (win_pos_open) {
			ImGuiEx::PrepareNextWindow(ImVec2(400, 155));
			ImGui::SetNextWindowDockID(dock_id, ImGuiCond_Once);
			if (ImGui::Begin(win_pos_name.c_str(), &win_pos_open)) {
				drawPosInfo();
			}
			ImGui::End();
		}
		if (win_debug_open) {
			if (ImGui::Begin(win_debug_name.c_str(), &win_debug_open)) {
				drawDebug();
			}
			ImGui::End();
		}
	}

}