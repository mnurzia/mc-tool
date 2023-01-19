#ifndef APP_COMPONENTS_WORLDVIEW_FRAGMENTMANAGER_HPP
#define APP_COMPONENTS_WORLDVIEW_FRAGMENTMANAGER_HPP

#include <cstdint>

#include <memory>
#include <vector>

#include <glad/glad.h>

#include "../../../worldgen/Generator.hpp"
#include "../../dispatchqueue/DispatchQueue.hpp"
#include "../../../util/UUID.hpp"
#include "Fragment.hpp"

namespace App {
	class App;
}

namespace App::Components::WorldView {

	class FragmentManager {

	private:
		int cache_size = 256;
		int texture_size = 1024;
		bool quitting = false;
		bool quit = false;
		bool rebuild_palette = false;
		GLuint palette_texture = 0;
		bool palette_enable[256];
		uint8_t palette_ocean_mode = 0;
		uint8_t palette_land_mode = 0;
		int64_t current_time = 0;
		WorldGen::Generator generator;
		App& app;
		std::vector<Fragment> cache;
		DispatchQueue::TaskGroup group;
		Util::UUID id;

	public:

		uint32_t palette_colors[256];

		FragmentManager(App& app_, int64_t seed);

		Fragment* getFragment(int32_t x, int32_t z);

		void buildPalette(uint8_t ocean_mode = 0, uint8_t land_mode = 0);

		void collectGarbage();
		
		void nextFrame();

		int getTextureSize();

		int getCacheSize();

		GLuint getPalette();

		uint32_t getPalette(uint8_t idx);

		void setPalette(uint8_t idx);

		void unsetPalette(uint8_t idx);

		bool getPaletteSet(uint8_t idx);

		uint8_t getPaletteMode(uint8_t idx);

		DispatchQueue::TaskGroup& getGroup();

		void setQuit(bool val);

		bool getQuit();

		void tryQuit();

		void destroy();
	};

}

#endif
