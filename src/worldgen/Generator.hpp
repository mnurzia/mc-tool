#ifndef WORLDGEN_GENERATOR_HPP
#define WORLDGEN_GENERATOR_HPP

#include <memory>
#include <string>
#include <vector>

#include "Biome.hpp"
#include "Feature.hpp"
#include "Layer.hpp"
#include "Map.hpp"

/** 
 * @file Generator.hpp
 *
 * @brief Biome data generator implementation.
 */ 

namespace WorldGen {

	/**
	 * @brief Generator access class.
	 *
	 * Wrapper around multiple Layers, managing connections between them and
	 * handling calls for their data.
	 */
	class Generator {

	public:

		/**
		 * @brief Describes the various usable minecraft versions.
		 */
		enum class Version : int32_t {
			MC_1_7,
			MC_1_8,
			MC_1_9,
			MC_1_10,
			MC_1_11,
			MC_1_12,
			MC_1_13,
			MC_1_14,
			MC_BEDROCK = 256
		};

		/**
		 * @brief Describes layer indexes/ids.
		 */
		enum class ID : int32_t {
			NONE = -1,
			ISLAND_4096 = 0,
			ZOOM_2048,
			ADD_ISLAND_2048,
			ZOOM_1024,
			ADD_ISLAND_1024A,
			ADD_ISLAND_1024B,
			ADD_ISLAND_1024C,
			REMOVE_OCEAN_1024,
			ADD_SNOW_1024,
			ADD_ISLAND_1024D,
			COOL_WARM_1024,
			HEAT_ICE_1024,
			SPECIAL_1024,
			ZOOM_512,
			ZOOM_256,
			ADD_ISLAND_256,
			ADD_MUSHROOM_256,
			DEEP_OCEAN_256,
			BIOME_256,
			ZOOM_128,
			ZOOM_64,
			BIOME_EDGE_64,
			RIVER_INIT_256,
			ZOOM_128_HILLS,
			ZOOM_64_HILLS,
			HILLS_64,
			RARE_BIOME_64,
			ZOOM_32,
			ADD_ISLAND_32,
			ZOOM_16,
			SHORE_16,
			ZOOM_8,
			ZOOM_4,
			SMOOTH_4,
			ZOOM_128_RIVER,
			ZOOM_64_RIVER,
			ZOOM_32_RIVER,
			ZOOM_16_RIVER,
			ZOOM_8_RIVER,
			ZOOM_4_RIVER,
			RIVER_4,
			SMOOTH_4_RIVER,
			RIVER_MIX_4,
			VORONOI_ZOOM_1,
			MC_1_13_OCEAN_TEMP_256,
			MC_1_13_ZOOM_128,
			MC_1_13_ZOOM_64,
			MC_1_13_ZOOM_32,
			MC_1_13_ZOOM_16,
			MC_1_13_ZOOM_8,
			MC_1_13_ZOOM_4,
			MC_1_13_OCEAN_MIX_4,
			MC_1_14_BAMBOO_256,
			COUNT
		};

		/**
		 * @brief Holds the individual layer descriptors.
		 *
		 * This vector is not traversed manually for generation, but it is used for
		 * enumeration and starting the generation (getMap is called on one member)
		 */
		std::vector<Layer> layers;

		/**
		 * @brief Minecraft version.
		 */
		Version version;

		/**
		 * @brief Seed.
		 */
		int64_t seed;

		/**
		 * @brief Setup the generator according to Minecraft version @c mcversion.
		 *
		 * Builds the @c layers vector and prepares each member for generation.
		 * The biomes list must be initialized before this is called.
		 *
		 * @param mc_version The minecraft version to use while generating.
		 */
		void init(Version mc_version);

		/**
		 * @brief Destroy the generator state.
		 */
		void destroy();

		/**
		 * @brief Initially used to set the seed (yes, this is the seed that
		 *        Minecraft uses)
		 *
		 * Internally calls the @b recursive function Layer::setWorldSeed().
		 */
		void applySeed(int64_t seed);

		/**
		 * @brief Retrieve the specified layer @c idx.
		 */
		Layer* getLayer(Generator::ID idx);

		/**
		 * @brief Get version.
		 */
		Version getVersion();

		/**
		 * @brief Get Minecraft seed.
		 */
		int64_t getSeed();

		/**
		 * @brief Get features
		 *
		 * Note that getFeatures will attempt to process all @b chunks in
		 * the region, not @b blocks. As such, structures may be generated
		 * up to 8 blocks around the outside of the region you want, and you
		 * will also have to size your data buffer to accomodate all chunks.
		 *
		 * @param feats The output buffer; all features are sent here.
		 * @param x The X coordinate of the starting region.
		 * @param z The Z coordinate of the starting region.
		 * @param w The width of the region.
		 * @param h The height of the region.
		 * @param data The raw biome data of the region.
		 */
		void getFeatures(
			std::vector<Feature>& feats,
			int32_t x,
			int32_t z,
			int32_t w,
			int32_t h,
			int32_t* data
		);
	private:

		void setupLayer(
			int32_t scale,
			Generator::ID layer_index,
			Generator::ID parent_index,
			int32_t seed,
			void (*getMap)(Layer* layer, int32_t* out, int32_t x, int32_t z, int32_t w, int32_t h)
		);

		void setupMultiLayer(
			int32_t scale,
			Generator::ID layer_index,
			Generator::ID parent_index,
			Generator::ID parent_index_2,
			int32_t seed,
			void (*getMap)(Layer* layer, int32_t* out, int32_t x, int32_t z, int32_t w, int32_t h)
		);

	};

}

#endif