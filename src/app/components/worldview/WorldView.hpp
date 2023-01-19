#ifndef APP_COMPONENTS_WORLDVIEW_WORLDVIEW_HPP
#define APP_COMPONENTS_WORLDVIEW_WORLDVIEW_HPP

#include "../../App.hpp"
#include "../UIComponent.hpp"
#include "FragmentManager.hpp"

#include <glm/glm.hpp>

namespace App::Components::WorldView {
	
	class WorldView : public UIComponent {

		FragmentManager fmgr;
		glm::vec2 offset = glm::vec2(0, 0);
		glm::vec2 click = glm::vec2(0, 0);
		glm::vec2 vel = glm::vec2(0, 0);
		glm::vec2 mouse = glm::vec2(0, 0);
		bool lock_offset = false;
		bool draw_grid = true;
		bool draw_grid_labels = true;
		bool draw_slime_chunks = false;
		bool draw_features[int(WorldGen::Feature::Type::COUNT)];
		bool drag_in = false;
		float friction = 0.95f;
		float zoom_want = 1.0f;
		float zoom_ratio = 1.0f;
		bool quitting = false;
		bool really_quit = false;
		bool frame_quit = true;
		bool win_pos_open = false;
		bool win_biome_open = false;
		bool win_debug_open = false;
		int rendered_frags = 0;
		int rendered_feats = 0;
		WorldGen::Feature selected_feature = WorldGen::Feature(0, 0, WorldGen::Feature::Type::NONE);

		std::string win_viewer_name;
		std::string win_pos_name;
		std::string win_debug_name;

		int64_t seed;
		WorldGen::Biome::ID current_biome = WorldGen::Biome::ID::THE_VOID;

		glm::vec2 toWorld(glm::vec2 in);

		glm::vec2 toScreen(glm::vec2 in);

	public:
		WorldView(App* app_, int64_t seed);

		bool quitLogic();

		void render();

		void drawMenuBar();

		void drawInfo();

		void drawGrid();

		void drawFragments();

		void drawXHair();

		void drawPosInfo();

		void drawDebug();

		void processInputs();

	};

}

#endif
