#ifndef APP_COMPONENTS_FONTMANAGER_HPP
#define APP_COMPONENTS_FONTMANAGER_HPP

#include <parallel_hashmap/phmap.h>

#include "../PersistentWindowComponent.hpp"

// Adapted from ImGui Example

namespace App::Components::FontManager {

	class FontManager : public PersistentWindowComponent {
		enum class FontBuildMode {
			FREETYPE = 0,
			STB = 1
		};

		FontBuildMode build_mode;
		bool          want_rebuild;
		float         font_multiply;
		int           font_padding;
		unsigned int  font_flags;
		float		  scale = 1.0f;
		std::vector<int> rect_ids;

		void reserveIconAtlas(int isize, int min, int max);
		void loadIconAtlas(std::string fname, int isize, int min, int max, int atl_width, int atl_height);

	public:
		FontManager(App* app);

		bool updateRebuild();

		void renderWin();
	};

}

#endif