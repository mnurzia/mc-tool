#include "FragmentManager.hpp"

#include <imgui/imgui.h>

#include "../../App.hpp"
#include "FragmentTask.hpp"
#include "../../../util/Color.hpp"

namespace App::Components::WorldView {

	FragmentManager::FragmentManager(App& app_, int64_t seed_) : app(app_)  {
		WorldGen::Biome::initBiomes();
		buildPalette();
		generator.init(WorldGen::Generator::Version::MC_1_14);
		generator.applySeed(seed_);
		cache.reserve(cache_size);
		for (int i = 0; i < cache_size; i++) {
			cache.push_back(Fragment(&generator, this));
		}
		group.setName("FragmentManager");
		app.getDispatchQueue().groupAdd(id, &group);
	}

	void FragmentManager::destroy() {
		glDeleteTextures(1, &palette_texture);
		generator.destroy();
		for (int i = 0; i < cache_size; i++) {
			cache[i].destroy();
		}
		app.getDispatchQueue().groupClear(id);
	}

	void FragmentManager::buildPalette(uint8_t ocean_mode, uint8_t land_mode) {
		palette_ocean_mode = ocean_mode;
		palette_land_mode = land_mode;
		for (int i = 0; i < 256; i++) {
			palette_colors[i] = Util::Color(WorldGen::Biome::biomes[i].amidst_color, 255);
			if (ocean_mode) {
				if (
					WorldGen::Biome::biomes[i].type_cat == WorldGen::Biome::TypeCat::OCEAN
					) {
					auto water_fog_color = Util::Color(WorldGen::Biome::biomes[i].water_fog_color, 255);
					auto water_color = Util::Color(WorldGen::Biome::biomes[i].water_color, 128);
					auto out = water_color + water_fog_color;
					palette_colors[i] = out;
				}
			}
			if (land_mode == 1) {
				if (
					WorldGen::Biome::biomes[i].type_cat == WorldGen::Biome::TypeCat::PLAINS ||
					WorldGen::Biome::biomes[i].type_cat == WorldGen::Biome::TypeCat::SWAMP ||
					WorldGen::Biome::biomes[i].type_cat == WorldGen::Biome::TypeCat::TAIGA ||
					WorldGen::Biome::biomes[i].type_cat == WorldGen::Biome::TypeCat::FOREST ||
					WorldGen::Biome::biomes[i].type_cat == WorldGen::Biome::TypeCat::JUNGLE ||
					WorldGen::Biome::biomes[i].type_cat == WorldGen::Biome::TypeCat::SAVANNA
				) {
					palette_colors[i] = Util::Color(WorldGen::Biome::biomes[i].grass_color, 255) * 0.8f;
				}
			} else if (land_mode == 2) {
				if (
					WorldGen::Biome::biomes[i].type_cat == WorldGen::Biome::TypeCat::PLAINS ||
					WorldGen::Biome::biomes[i].type_cat == WorldGen::Biome::TypeCat::SWAMP ||
					WorldGen::Biome::biomes[i].type_cat == WorldGen::Biome::TypeCat::TAIGA ||
					WorldGen::Biome::biomes[i].type_cat == WorldGen::Biome::TypeCat::FOREST ||
					WorldGen::Biome::biomes[i].type_cat == WorldGen::Biome::TypeCat::JUNGLE ||
					WorldGen::Biome::biomes[i].type_cat == WorldGen::Biome::TypeCat::SAVANNA
					) {
					palette_colors[i] = Util::Color(WorldGen::Biome::biomes[i].foliage_color, 255);
				}
			}
			if (land_mode) {
				if (
					WorldGen::Biome::biomes[i].type_cat == WorldGen::Biome::TypeCat::DESERT
				) {
					palette_colors[i] = Util::Color(213, 196, 150);
				}
			}
			palette_enable[i] = true;
		}
		rebuild_palette = true;
	}

	void FragmentManager::setPalette(uint8_t idx) {
		// this is redundant ik ok
		palette_colors[idx] = (palette_colors[idx] & ~(0xFF000000)) | 0xFF000000;
		palette_enable[idx] = true;
		rebuild_palette = true;
	}

	void FragmentManager::unsetPalette(uint8_t idx) { 
		palette_colors[idx] = (palette_colors[idx] & ~(0xFF000000)) | 0x28000000;
		palette_enable[idx] = false;
		rebuild_palette = true;
	}

	uint32_t FragmentManager::getPalette(uint8_t idx) {
		return palette_colors[idx];
	}

	bool FragmentManager::getPaletteSet(uint8_t idx) {
		return palette_enable[idx];
	}

	uint8_t FragmentManager::getPaletteMode(uint8_t idx) {
		if (idx) {
			return palette_land_mode;
		} else {
			return palette_ocean_mode;
		}
	}

	Fragment* FragmentManager::getFragment(int32_t x, int32_t z) {
		if (quitting) {
			return NULL;
		}
		if (palette_texture == 0) {
			glGenTextures(1, &palette_texture);
			glBindTexture(GL_TEXTURE_1D, palette_texture);
			glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexImage1D(GL_TEXTURE_1D, 0, GL_RGBA, 256, 0, GL_RGBA, GL_UNSIGNED_BYTE, &palette_colors);
			rebuild_palette = false;
		} else if (rebuild_palette) {
			glBindTexture(GL_TEXTURE_1D, palette_texture);
			glTexSubImage1D(GL_TEXTURE_1D, 0, 0, 256, GL_RGBA, GL_UNSIGNED_BYTE, &palette_colors);
			rebuild_palette = false;
		}
		int64_t lowest_time = current_time - 1;
		int lowest_idx = -1;
		int non_idx = -1;
		for (int i = 0; i < cache_size; i++) {
			Fragment& current = cache[i];
			Fragment::State current_state = current.getState();
			switch (current_state) {
				case Fragment::State::UNINITIALIZED:
					non_idx = i;
					if (current.getAccess() < lowest_time) {
						lowest_time = current.getAccess();
						lowest_idx = i;
					}
					break;
				case Fragment::State::PROCESSING:
					if (current.getX() == x && current.getZ() == z) {
						current.setAccess(current_time);
						return NULL;
					}
					break;
				case Fragment::State::GENERATED:
				case Fragment::State::BOUND:
					if (current.getX() == x && current.getZ() == z) {
						if (current_state == Fragment::State::GENERATED) {
							//current.setTexture(app.bindTexture(current.getTexture(), current.getData(), texture_size, texture_size));
							current.bindTexture();
							current.setState(Fragment::State::BOUND);
						}
						current.setAccess(current_time);
						return &cache[i];
					} else {
						if (current.getAccess() < lowest_time) {
							lowest_time = current.getAccess();
							lowest_idx = i;
						}
					}
					break;
			}
		}

		Fragment* candidate = NULL;
		if (lowest_idx == -1) {
			// Cache full
			return NULL;
		} else {
			if (non_idx == -1) {
				candidate = &cache[lowest_idx];
			} else {
				candidate = &cache[non_idx];
			}
		}

		candidate->setX(x);
		candidate->setZ(z);
		candidate->setAccess(current_time);
		candidate->setState(Fragment::State::PROCESSING);

		std::unique_ptr<DispatchQueue::Task> task = std::make_unique<FragmentTask>(candidate);
		app.getDispatchQueue().dispatch(std::move(task));

		return NULL; // not done processing
	}

	void FragmentManager::collectGarbage() {
		for (int i = 0; i < cache_size; i++) {
			Fragment& current = cache[i];
			if (current.getAccess() < current_time) {
				if (current.getState() == Fragment::State::BOUND) {
					//app.unbindTexture(current.getTexture());
					current.unbindTexture();
					current.setState(Fragment::State::GENERATED);
					//current.setTexture(0);
				}
			}
		}
	}

	void FragmentManager::nextFrame() {
		current_time++;
	}

	int FragmentManager::getTextureSize() {
		return texture_size;
	}

	int FragmentManager::getCacheSize() {
		return cache_size;
	}

	GLuint FragmentManager::getPalette() {
		return palette_texture;
	}

	DispatchQueue::TaskGroup& FragmentManager::getGroup() {
		return group;
	}

	void FragmentManager::setQuit(bool val) {
		quitting = true;
	}

	bool FragmentManager::getQuit() {
		return quit;
	}

	void FragmentManager::tryQuit() {
		for (int i = 0; i < cache_size; i++) {
			if (cache[i].getState() == Fragment::State::PROCESSING) {
				return;
			}
		}
		quit = true;
	}

}