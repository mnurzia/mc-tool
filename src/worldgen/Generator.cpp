#include "Generator.hpp"

#include <glm/vec2.hpp>

#include "JavaRandom.hpp"

namespace WorldGen {

	const int32_t FEATURE_TREASURE_SEED = 10387320;
	const int32_t FEATURE_PYRAMID_SEED = 14357617;
	const int32_t FEATURE_IGLOO_SEED = 14357618;
	const int32_t FEATURE_JUNGLE_TEMPLE_SEED = 14357619;
	const int32_t FEATURE_OCEAN_MONUMENT_SEED = 10387313;
	const int32_t FEATURE_OCEAN_RUIN_SEED = 14357621;
	const int32_t FEATURE_PILLAGER_OUTPOST_SEED = 165745296;
	const int32_t FEATURE_SHIPWRECK_SEED = 165745295;
	const int32_t FEATURE_SWAMP_HUT_SEED = 14357620;
	const int32_t FEATURE_VILLAGE_SEED = 10387312;
	const int32_t FEATURE_WOODLAND_MANSION_SEED = 10387319;

	void Generator::setupMultiLayer(
		int32_t scale,
		Generator::ID layer_index,
		Generator::ID parent_index,
		Generator::ID parent_index_2,
		int32_t seed,
		void (*getMap)(Layer* layer, int32_t* out, int32_t x, int32_t z, int32_t w, int32_t h)
	) {
		Layer* lay = &(layers[int32_t(layer_index)]);
		lay->setBaseSeed(seed);
		lay->scale = scale;
		if (parent_index == Generator::ID::NONE) {
			lay->parent = NULL;
		}
		else {
			lay->parent = &(layers[int32_t(parent_index)]);
		}
		if (parent_index_2 == Generator::ID::NONE) {
			lay->parent_2 = NULL;
		}
		else {
			lay->parent_2 = &(layers[int32_t(parent_index_2)]);
		}
		lay->getMap = getMap;
		lay->ocean_gen = NULL;
	}

	void Generator::setupLayer(
		int32_t scale,
		Generator::ID layer_index,
		Generator::ID parent_index,
		int32_t seed,
		void (*getMap)(Layer* layer, int32_t* out, int32_t x, int32_t z, int32_t w, int32_t h)
	) {
		setupMultiLayer(scale, layer_index, parent_index, Generator::ID::NONE, seed, getMap);
	}

	void Generator::init(Version mc_version) {
		version = mc_version;
		if (Biome::biomes[Biome::ID::PLAINS].id == 0) {
			printf("Ensure that Biome::initBiomes() is called before Generator::init()!\n");
			exit(1);
		}
		for (int32_t i = 0; i < int32_t(ID::COUNT); i++) {
			Layer l;
			l.parent = NULL;
			l.parent_2 = NULL;
			l.scale = 0;
			l.setBaseSeed(0);
			l.ocean_gen = NULL;
			layers.push_back(l);
		}

		setupLayer(4096, ID::ISLAND_4096, ID::NONE, 1, Map::island);
		setupLayer(2048, ID::ZOOM_2048, ID::ISLAND_4096, 2000, Map::zoom);
		setupLayer(2048, ID::ADD_ISLAND_2048, ID::ZOOM_2048, 1, Map::addIsland);
		setupLayer(1024, ID::ZOOM_1024, ID::ADD_ISLAND_2048, 2001, Map::zoom);
		setupLayer(1024, ID::ADD_ISLAND_1024A, ID::ZOOM_1024, 2, Map::addIsland);
		setupLayer(1024, ID::ADD_ISLAND_1024B, ID::ADD_ISLAND_1024A, 50, Map::addIsland);
		setupLayer(1024, ID::ADD_ISLAND_1024C, ID::ADD_ISLAND_1024B, 70, Map::addIsland);
		setupLayer(1024, ID::REMOVE_OCEAN_1024, ID::ADD_ISLAND_1024C, 2, Map::removeTooMuchOcean);

		setupLayer(1024, ID::ADD_SNOW_1024, ID::REMOVE_OCEAN_1024, 2, Map::addSnow);
		setupLayer(1024, ID::ADD_ISLAND_1024D, ID::ADD_SNOW_1024, 3, Map::addIsland);
		setupLayer(1024, ID::COOL_WARM_1024, ID::ADD_ISLAND_1024D, 2, Map::coolWarm);
		setupLayer(1024, ID::HEAT_ICE_1024, ID::COOL_WARM_1024, 2, Map::heatIce);
		setupLayer(1024, ID::SPECIAL_1024, ID::HEAT_ICE_1024, 3, Map::special);
		setupLayer(512, ID::ZOOM_512, ID::SPECIAL_1024, 2002, Map::zoom);
		setupLayer(256, ID::ZOOM_256, ID::ZOOM_512, 2003, Map::zoom);
		setupLayer(256, ID::ADD_ISLAND_256, ID::ZOOM_256, 4, Map::addIsland);
		setupLayer(256, ID::ADD_MUSHROOM_256, ID::ADD_ISLAND_256, 5, Map::addMushroomIsland);
		setupLayer(256, ID::DEEP_OCEAN_256, ID::ADD_MUSHROOM_256, 4, Map::addDeepOcean);

		setupLayer(256, ID::BIOME_256, ID::DEEP_OCEAN_256, 200,
			mc_version != Version::MC_BEDROCK ? Map::biome : Map::biomeBE
		);

		if (mc_version <= Version::MC_1_13) {
			setupLayer(128, ID::ZOOM_128, ID::BIOME_256, 1000, Map::zoom);
		}
		else {
			setupLayer(256, ID::MC_1_14_BAMBOO_256, ID::BIOME_256, 1001, Map::addBamboo);
			setupLayer(128, ID::ZOOM_128, ID::MC_1_14_BAMBOO_256, 1000, Map::zoom);
		}

		setupLayer(64, ID::ZOOM_64, ID::ZOOM_128, 1001, Map::zoom);
		setupLayer(64, ID::BIOME_EDGE_64, ID::ZOOM_64, 1000, Map::biomeEdge);

		setupLayer(256, ID::RIVER_INIT_256, ID::DEEP_OCEAN_256, 100, Map::riverInit);
		setupLayer(128, ID::ZOOM_128_HILLS, ID::RIVER_INIT_256, 1000, Map::zoom);
		setupLayer(64, ID::ZOOM_64_HILLS, ID::ZOOM_128_HILLS, 1001, Map::zoom);

		setupMultiLayer(
			64, ID::HILLS_64, ID::BIOME_EDGE_64,
			ID::ZOOM_64_HILLS, 1000,
			mc_version <= Version::MC_1_12 ? Map::hills : Map::hills113
		);

		setupLayer(64, ID::RARE_BIOME_64, ID::HILLS_64, 1001, Map::rareBiome);
		setupLayer(32, ID::ZOOM_32, ID::RARE_BIOME_64, 1000, Map::zoom);
		setupLayer(32, ID::ADD_ISLAND_32, ID::ZOOM_32, 3, Map::addIsland);
		setupLayer(16, ID::ZOOM_16, ID::ADD_ISLAND_32, 1001, Map::zoom);
		setupLayer(16, ID::SHORE_16, ID::ZOOM_16, 1000, Map::shore);
		setupLayer(8, ID::ZOOM_8, ID::SHORE_16, 1002, Map::zoom);
		setupLayer(4, ID::ZOOM_4, ID::ZOOM_8, 1003, Map::zoom);
		setupLayer(4, ID::SMOOTH_4, ID::ZOOM_4, 1000, Map::smooth);

		// river layer chain
		setupLayer(128, ID::ZOOM_128_RIVER, ID::RIVER_INIT_256, 1000, Map::zoom);
		setupLayer(64, ID::ZOOM_64_RIVER, ID::ZOOM_128_RIVER, 1001, Map::zoom);
		setupLayer(32, ID::ZOOM_32_RIVER, ID::ZOOM_64_RIVER, 1000, Map::zoom);
		setupLayer(16, ID::ZOOM_16_RIVER, ID::ZOOM_32_RIVER, 1001, Map::zoom);
		setupLayer(8, ID::ZOOM_8_RIVER, ID::ZOOM_16_RIVER, 1002, Map::zoom);
		setupLayer(4, ID::ZOOM_4_RIVER, ID::ZOOM_8_RIVER, 1003, Map::zoom);
		setupLayer(4, ID::RIVER_4, ID::ZOOM_4_RIVER, 1, Map::river);
		setupLayer(4, ID::SMOOTH_4_RIVER, ID::RIVER_4, 1000, Map::smooth);

		setupMultiLayer(
			4, ID::RIVER_MIX_4, ID::SMOOTH_4,
			ID::SMOOTH_4_RIVER, 100, Map::riverMix
		);

		if (mc_version <= Version::MC_1_12) {
			setupLayer(1, ID::VORONOI_ZOOM_1, ID::RIVER_MIX_4, 10, Map::voronoiZoom);
		}
		else {
			setupLayer(
				256, ID::MC_1_13_OCEAN_TEMP_256,
				ID::NONE, 2, Map::oceanTemp
			);
			layers[int32_t(ID::MC_1_13_OCEAN_TEMP_256)].ocean_gen = (OceanGen*)malloc(sizeof(OceanGen));
			setupLayer(
				128, ID::MC_1_13_ZOOM_128, ID::MC_1_13_OCEAN_TEMP_256,
				2001, Map::zoom
			);
			setupLayer(64, ID::MC_1_13_ZOOM_64, ID::MC_1_13_ZOOM_128, 2002, Map::zoom);
			setupLayer(32, ID::MC_1_13_ZOOM_32, ID::MC_1_13_ZOOM_64, 2003, Map::zoom);
			setupLayer(16, ID::MC_1_13_ZOOM_16, ID::MC_1_13_ZOOM_32, 2004, Map::zoom);
			setupLayer(8, ID::MC_1_13_ZOOM_8, ID::MC_1_13_ZOOM_16, 2005, Map::zoom);
			setupLayer(4, ID::MC_1_13_ZOOM_4, ID::MC_1_13_ZOOM_8, 2006, Map::zoom);

			setupMultiLayer(
				4, ID::MC_1_13_OCEAN_MIX_4, ID::RIVER_MIX_4,
				ID::MC_1_13_ZOOM_4, 100, Map::oceanMix);

			setupLayer(1, ID::VORONOI_ZOOM_1, ID::MC_1_13_OCEAN_MIX_4,
				10, Map::voronoiZoom);
		}
	}

	void Generator::destroy() {
		for (auto e : layers) {
			if (e.ocean_gen != nullptr) {
				free(e.ocean_gen);
			}
		}
	}

	void Generator::applySeed(int64_t seed) {
		this->seed = seed;
		// the seed has to be applied recursively
		layers[int32_t(ID::VORONOI_ZOOM_1)].setWorldSeed(seed);
	}

	Layer* Generator::getLayer(Generator::ID idx) {
		return &layers[(int32_t)(idx)];
	}

	Generator::Version Generator::getVersion() {
		return version;
	}

	int64_t Generator::getSeed() {
		return seed;
	}

	glm::ivec2 featureStart(
		int32_t chunk_x,
		int32_t chunk_z,
		int32_t spacing,
		int32_t separation,
		int32_t magic_seed,
		int64_t world_seed
	) {
		int32_t factor0 = (chunk_x < 0) ? (chunk_x - spacing + 1) : chunk_x;
		int32_t factor1 = (chunk_z < 0) ? (chunk_z - spacing + 1) : chunk_z;
		int32_t factor2 = factor0 / spacing;
		int32_t factor3 = factor1 / spacing;
		int64_t structure_seed = JavaRandom::setStructureSeed(world_seed, factor2, factor3, magic_seed);
		factor2 *= spacing;
		factor3 *= spacing;
		factor2 += JavaRandom::nextInt(&structure_seed, spacing - separation);
		factor3 += JavaRandom::nextInt(&structure_seed, spacing - separation);
		return glm::ivec2(factor2, factor3);
	}

	static bool featureAtChunk(
		int32_t chunk_x,
		int32_t chunk_z,
		int32_t spacing,
		int32_t separation,
		int32_t magic_seed,
		int64_t world_seed
	) {
		return (featureStart(chunk_x, chunk_z, spacing, separation, magic_seed, world_seed) == glm::ivec2(chunk_x, chunk_z));
	}

	static bool featureAtChunkExAlgo(
		int32_t chunk_x,
		int32_t chunk_z,
		int32_t spacing,
		int32_t separation,
		int32_t magic_seed,
		int64_t world_seed
	) {
		int32_t factor0 = (chunk_x < 0) ? (chunk_x - spacing + 1) : chunk_x;
		int32_t factor1 = (chunk_z < 0) ? (chunk_z - spacing + 1) : chunk_z;
		int32_t factor2 = factor0 / spacing;
		int32_t factor3 = factor1 / spacing;
		int64_t structure_seed = JavaRandom::setStructureSeed(world_seed, factor2, factor3, magic_seed);
		factor2 *= spacing;
		factor3 *= spacing;
		factor2 += (
			JavaRandom::nextInt(&structure_seed, spacing - separation) + 
			JavaRandom::nextInt(&structure_seed, spacing - separation)
		) / 2;
		factor3 += (
			JavaRandom::nextInt(&structure_seed, spacing - separation) +
			JavaRandom::nextInt(&structure_seed, spacing - separation)
		) / 2;
		return (factor2 == chunk_x && factor3 == chunk_z);
	}

	static bool featureAtChunkOceanMonument(
		int32_t chunk_x,
		int32_t chunk_z,
		int64_t world_seed
	) {
		if (featureAtChunkExAlgo(chunk_x, chunk_z, 32, 5, FEATURE_OCEAN_MONUMENT_SEED, world_seed)) {
			Generator temp;
			temp.init(Generator::Version::MC_1_14);
			temp.applySeed(world_seed);
			Layer* top = temp.getLayer(Generator::ID::VORONOI_ZOOM_1);
			int32_t x_pos = (chunk_x << 4) + 9;
			int32_t z_pos = (chunk_z << 4) + 9;
			int32_t x_top = x_pos - 16;
			int32_t z_top = z_pos - 16;
			int32_t x_bot = x_pos + 16;
			int32_t z_bot = z_pos + 16;
			int32_t c_width = x_bot - x_top + 1;
			int32_t c_height = z_bot - z_top + 1;
			int32_t* out = top->allocCache(c_width, c_height);
			top->genArea(out, x_top, z_top, c_width, c_height);
			for (int32_t z_off = z_top; z_off < z_top + c_height; z_off++) {
				for (int32_t x_off = x_top; x_off < x_top + c_width; x_off++) {
					auto pos = glm::ivec2(
						(x_off - x_top),
						(z_off - z_top)
					);
					if (!Biome::canFeatureSpawn(Biome::ID(out[pos.x + pos.y * c_width]), Feature::Type::OCEAN_MONUMENT)) {
						temp.destroy();
						free(out);
						return false;
					}
				}
			}
			free(out);
			x_top = x_pos - 29;
			z_top = z_pos - 29;
			x_bot = x_pos + 29;
			z_bot = z_pos + 29;
			c_width = x_bot - x_top + 1;
			c_height = z_bot - z_top + 1;
			out = top->allocCache(c_width, c_height);
			top->genArea(out, x_top, z_top, c_width, c_height);
			for (int32_t z_off = z_top; z_off < z_top + c_height; z_off++) {
				for (int32_t x_off = x_top; x_off < x_top + c_width; x_off++) {
					auto pos = glm::ivec2(
						(x_off - x_top),
						(z_off - z_top)
					);
					Biome::ID cur = Biome::ID(out[pos.x + pos.y * c_width]);
					if (Biome::getTypeCat(cur) != Biome::TypeCat::OCEAN && Biome::getTypeCat(cur) != Biome::TypeCat::RIVER) {
						temp.destroy();
						free(out);
						return false;
					}
				}
			}
			temp.destroy();
			free(out);
			return true;
		}
		return false;
	}

	static bool featureAtChunkPillagerOutpost(
		int32_t chunk_x,
		int32_t chunk_z,
		int64_t world_seed
	) {
		if (featureStart(chunk_x, chunk_z, 32, 8, FEATURE_PILLAGER_OUTPOST_SEED, world_seed) == glm::ivec2(chunk_x, chunk_z)) {
			int32_t factor4 = chunk_x >> 4;
			int32_t factor5 = chunk_z >> 4;
			int64_t temp_seed = (int64_t)(factor4 ^ factor5 << 4) ^ world_seed;
			JavaRandom::setSeed(&temp_seed);
			JavaRandom::next(&temp_seed, 32);
			// 1 in 5 chance of doing an expensive generation. I like those odds!
			if (JavaRandom::nextInt(&temp_seed, 5) != 0) {
				return false;
			}
			Generator temp;
			temp.init(Generator::Version::MC_1_14);
			temp.applySeed(world_seed);
			Layer* top = temp.getLayer(Generator::ID::VORONOI_ZOOM_1);
			int32_t c_width = 20 * 16 + 1;
			int32_t c_height = 20 * 16 + 1;
			int32_t* out = top->allocCache(c_width, c_height);
			top->genArea(out, ((chunk_x << 4) + 9) - 160, ((chunk_z << 4) + 9) - 160, c_width, c_height);
			for (int32_t z_off = chunk_z - 10; z_off <= chunk_z + 10; ++z_off) {
				for (int32_t x_off = chunk_x - 10; x_off <= chunk_x + 10; ++x_off) {
					auto pos = glm::ivec2(
						((x_off - chunk_x + 10) << 4),
						((z_off - chunk_z + 10) << 4)
					);
					if (Biome::canFeatureSpawn((Biome::ID)out[pos.x + pos.y * c_width], Feature::Type::VILLAGE)) {
						if (featureAtChunk(x_off, z_off, 32, 8, FEATURE_VILLAGE_SEED, world_seed)) {
							temp.destroy();
							free(out);
							return false;
						}
					}
				}
			}
			temp.destroy();
			free(out);
			return true;
		} else {
			return false;
		}
	}

	static bool featureAtChunkWoodlandMansion(
		int32_t chunk_x,
		int32_t chunk_z,
		int64_t world_seed
	) {
		if (featureAtChunkExAlgo(chunk_x, chunk_z, 80, 20, FEATURE_WOODLAND_MANSION_SEED, world_seed)) {
			Generator temp;
			temp.init(Generator::Version::MC_1_14);
			temp.applySeed(world_seed);
			Layer* top = temp.getLayer(Generator::ID::VORONOI_ZOOM_1);
			int32_t x_pos = (chunk_x << 4) + 9;
			int32_t z_pos = (chunk_z << 4) + 9;
			int32_t x_top = x_pos - 32;
			int32_t z_top = z_pos - 32;
			int32_t x_bot = x_pos + 32;
			int32_t z_bot = z_pos + 32;
			int32_t c_width = x_bot - x_top + 1;
			int32_t c_height = z_bot - z_top + 1;
			int32_t* out = top->allocCache(c_width, c_height);
			top->genArea(out, x_top, z_top, c_width, c_height);
			for (int32_t z_off = z_top; z_off < z_top + c_height; z_off++) {
				for (int32_t x_off = x_top; x_off < x_top + c_width; x_off++) {
					auto pos = glm::ivec2(
						(x_off - x_top),
						(z_off - z_top)
					);
					if (!Biome::canFeatureSpawn(Biome::ID(out[pos.x + pos.y * c_width]), Feature::Type::WOODLAND_MANSION)) {
						temp.destroy();
						free(out);
						return false;
					}
				}
			}
			temp.destroy();
			free(out);
			return true;
		}
		return false;
	}

	void Generator::getFeatures(
		std::vector<Feature>& feats,
		int32_t x,
		int32_t z,
		int32_t w,
		int32_t h,
		int32_t* data
	) {
		// Round to multiples of 16 (round up for end_x + end_z)
		int32_t start_x = x >> 4;
		int32_t start_z = z >> 4;
		int32_t end_x   = ((x + w) + 15) >> 4;
		int32_t end_z   = ((z + h) + 15) >> 4;
		for (int32_t zi = start_z; zi < end_z; zi++) {
			for (int32_t xi = start_x; xi < end_x; xi++) {
				int32_t struct_x = xi * 16 + 9;
				int32_t struct_z = zi * 16 + 9;
				Biome::ID struct_biome = (Biome::ID)data[(struct_x - x) + (struct_z - z) * w];
				int64_t gen_seed = getSeed();
				int64_t temp_seed;
				// Mineshafts
				if (Biome::canFeatureSpawn(struct_biome, Feature::Type::MINESHAFT)) {
					temp_seed = gen_seed;
					JavaRandom::setSeed(&temp_seed);
					int64_t i = JavaRandom::nextLong(&temp_seed);
					int64_t j = JavaRandom::nextLong(&temp_seed);
					temp_seed = xi * i ^ zi * j ^ gen_seed;
					JavaRandom::setSeed(&temp_seed);
					if (JavaRandom::nextDouble(&temp_seed) < 0.004) {
						feats.push_back(Feature(struct_x, struct_z, Feature::Type::MINESHAFT));
					}
				}
				// Buried Treasure
				if (Biome::canFeatureSpawn(struct_biome, Feature::Type::BURIED_TREASURE)) {
					temp_seed = gen_seed;
					temp_seed = xi * 341873128712 + zi * 132897987541 + gen_seed + FEATURE_TREASURE_SEED;
					JavaRandom::setSeed(&temp_seed);
					if (JavaRandom::nextFloat(&temp_seed) < 0.01) {
						feats.push_back(Feature(struct_x, struct_z, Feature::Type::BURIED_TREASURE));
					}
				}
				// Desert Temples
				if (Biome::canFeatureSpawn(struct_biome, Feature::Type::DESERT_PYRAMID)) {
					if (featureAtChunk(xi, zi, 32, 8, FEATURE_PYRAMID_SEED, gen_seed)) {
						feats.push_back(Feature(struct_x, struct_z, Feature::Type::DESERT_PYRAMID));
					}
				}
				// Igloos
				if (Biome::canFeatureSpawn(struct_biome, Feature::Type::IGLOO)) {
					if (featureAtChunk(xi, zi, 32, 8, FEATURE_IGLOO_SEED, gen_seed)) {
						feats.push_back(Feature(struct_x, struct_z, Feature::Type::IGLOO));
					}
				}
				// Jungle Temples
				if (Biome::canFeatureSpawn(struct_biome, Feature::Type::JUNGLE_TEMPLE)) {
					if (featureAtChunk(xi, zi, 32, 8, FEATURE_JUNGLE_TEMPLE_SEED, gen_seed)) {
						feats.push_back(Feature(struct_x, struct_z, Feature::Type::JUNGLE_TEMPLE));
					}
				}
				// Ocean Monuments
				if (Biome::canFeatureSpawn(struct_biome, Feature::Type::OCEAN_MONUMENT)) {
					if (featureAtChunkOceanMonument(xi, zi, gen_seed)) {
						feats.push_back(Feature(struct_x, struct_z, Feature::Type::OCEAN_MONUMENT));
					}
				}
				// Ocean Ruins
				if (Biome::canFeatureSpawn(struct_biome, Feature::Type::OCEAN_RUIN)) {
					if (featureAtChunk(xi, zi, 16, 8, FEATURE_OCEAN_RUIN_SEED, gen_seed)) {
						feats.push_back(Feature(struct_x, struct_z, Feature::Type::OCEAN_RUIN));
					}
				}
				// Pillager Outposts
				if (Biome::canFeatureSpawn(struct_biome, Feature::Type::PILLAGER_OUTPOST)) {
					if (featureAtChunkPillagerOutpost(xi, zi, gen_seed)) {
						feats.push_back(Feature(struct_x, struct_z, Feature::Type::PILLAGER_OUTPOST));
					}
				}
				// Shipwrecks
				if (Biome::canFeatureSpawn(struct_biome, Feature::Type::SHIPWRECK)) {
					if (featureAtChunk(xi, zi, 16, 8, FEATURE_SHIPWRECK_SEED, gen_seed)) {
						feats.push_back(Feature(struct_x, struct_z, Feature::Type::SHIPWRECK));
					}
				}
				// Swamp Huts
				if (Biome::canFeatureSpawn(struct_biome, Feature::Type::SWAMP_HUT)) {
					if (featureAtChunk(xi, zi, 32, 8, FEATURE_SWAMP_HUT_SEED, gen_seed)) {
						feats.push_back(Feature(struct_x, struct_z, Feature::Type::SWAMP_HUT));
					}
				}
				// Villages
				if (Biome::canFeatureSpawn(struct_biome, Feature::Type::VILLAGE)) {
					if (featureAtChunk(xi, zi, 32, 8, FEATURE_VILLAGE_SEED, gen_seed)) {
						feats.push_back(Feature(struct_x, struct_z, Feature::Type::VILLAGE));
					}
				}
				// Woodland Mansions
				if (Biome::canFeatureSpawn(struct_biome, Feature::Type::WOODLAND_MANSION)) {
					if (featureAtChunkWoodlandMansion(xi, zi, gen_seed)) {
						feats.push_back(Feature(struct_x, struct_z, Feature::Type::WOODLAND_MANSION));
					}
				}
			}
		}
	}
}