#ifndef WORLDGEN_LAYER_HPP
#define WORLDGEN_LAYER_HPP

#include <cstdint>

#include <string>

#include "OceanGen.hpp"

/** 
 * @file Layer.hpp
 *
 * @brief Generator layer operations and generation.
 */ 

/**
 * @brief Layer access and traversal class.
 *
 * The core of world generation; layers are traversed and @c getMap is called
 * on each one in order to compose a "map" of output biomes.
 */
namespace WorldGen {

	class Layer {
	public:
		//std::string name;        /*!< @brief Layer name (used for debugging) */
		int64_t     base_seed;   /*!< @brief Generator seed (depends only on hierarchy) */
		int64_t     world_seed;  /*!< @brief Derived from (not equal to) the seed of the world */
		int64_t     chunk_seed;  /*!< @brief Chunk randomiser seed */
		int         scale;       /*!< @brief Map scale of this layer (output = scale * scale blocks) */
		Layer* parent;      /*!< @brief Parent layer (required) */
		Layer* parent_2;    /*!< @brief Parent layer (optioinal) */

		/**
		 * @brief Function pointer used to populate buffer @c out
		 *
		 * Called as the layer hierarchy is traversed. A virtual function was
		 * intentionally avoided for speed reasons.
		 *
		 * @param out The output buffer.
		 * @param x The starting x coordinate.
		 * @param y The starting y coordinate.
		 * @param w The amount of blocks on the x axis to generate.
		 * @param h The amount of blocks on the y axis to generate.
		 */
		void (*getMap)(Layer* layer, int32_t* out, int32_t x, int32_t z, int32_t w, int32_t h);

		OceanGen* ocean_gen = nullptr; /*!< @brief Ocean temperature generator state */

		/**
		 * @brief Return the next random integer identically to Minecraft's technique.
		 *
		 * Create a range using @c mod (the output seed is moduloed against it);
		 * randint() can easily be emulated using this technique.
		 * Modifies the internal value of chunk_seed in order to yield a different
		 * result every call.
		 *
		 * @param mod The upper bound of the generated number.
		 *
		 * @return The next integer output of the PRNG.
		 */
		inline int32_t mcNextInt(int32_t mod) {
			int32_t ret = (int32_t)((this->chunk_seed >> 24) % (int64_t)mod);

			if (ret < 0) {
				ret += mod;
			}

			chunk_seed *= chunk_seed * 6364136223846793005LL + 1442695040888963407LL;
			chunk_seed += world_seed;
			return ret;
		}

		/**
		 * @brief Set @c chunk_seed according to @c chunk_x and @c chunk_z.
		 *
		 * Modifies the internal value of chunk_seed based on @c chunk_x and
		 * @c chunk_z, using 4 MACs and 4 adds. Pretty efficient.
		 *
		 * @param chunk_x The chunk's X coordinate
		 * @param chunk_z The chunk's Z coordinate
		 *
		 * @todo SIMD support
		 */
		inline void setChunkSeed(int64_t chunk_x, int64_t chunk_z) {
			chunk_seed = world_seed;
			chunk_seed *= chunk_seed * 6364136223846793005LL + 1442695040888963407LL;
			chunk_seed += chunk_x;
			chunk_seed *= chunk_seed * 6364136223846793005LL + 1442695040888963407LL;
			chunk_seed += chunk_z;
			chunk_seed *= chunk_seed * 6364136223846793005LL + 1442695040888963407LL;
			chunk_seed += chunk_x;
			chunk_seed *= chunk_seed * 6364136223846793005LL + 1442695040888963407LL;
			chunk_seed += chunk_z;
		}

		/**
		 * @brief Set @c base_seed according to @c seed.
		 *
		 * Modifies the internal value of base_seed according to @c seed using
		 * Minecraft's algorithm involving multiplying and adding by magic numbers.
		 * Also nullifies @c parent.
		 *
		 * @param seed The input seed to be used
		 *
		 * @todo SIMD support
		 */
		inline void setBaseSeed(int64_t seed) {
			base_seed = seed;
			base_seed *= base_seed * 6364136223846793005LL + 1442695040888963407LL;
			base_seed += seed;
			base_seed *= base_seed * 6364136223846793005LL + 1442695040888963407LL;
			base_seed += seed;
			base_seed *= base_seed * 6364136223846793005LL + 1442695040888963407LL;
			base_seed += seed;
			parent = nullptr;
			world_seed = 0;
			chunk_seed = 0;
		}

		/**
		 * @brief Select either @c a1 or @c a2.
		 *
		 * Utilizes mcNextInt() to select one or the other.
		 *
		 * @param a1 First choice
		 * @param a2 Second choice
		 *
		 * @return One of (a1, a2)
		 *
		 * @todo SIMD support
		 */
		inline int32_t selectRandom2(int32_t a1, int32_t a2) {
			int32_t i = mcNextInt(2);
			return i == 0 ? a1 : a2;
		}

		/**
		 * @brief Select either @c a1, @c a2, @c a3, or @c a4
		 *
		 * Utilizes mcNextInt() to select one with equal probability.
		 *
		 * @param a1 First choice
		 * @param a2 Second choice
		 * @param a3 Third choice
		 * @param a4 Fourth choice
		 *
		 * @return One of (a1, a2, a3, a4)
		 *
		 * @todo SIMD support
		 */
		inline int32_t selectRandom4(int32_t a1, int32_t a2, int32_t a3, int32_t a4) {
			int32_t i = mcNextInt(4);
			return i == 0 ? a1 : i == 1 ? a2 : i == 2 ? a3 : a4;
		}

		/**
		 * @brief Attempt to find the minimum buffer dimensions needed to represent
		 *        the area specified.
		 *
		 * @param area_x area X coordinate
		 * @param area_z area Z coordinate
		 * @param max_x area width (also output width)
		 * @param max_z area height (also output height)
		 */
		void getMaxArea(int32_t area_x, int32_t area_z, int32_t* max_x, int32_t* max_z);

		/**
		 * @brief Calculate the required amount of memory needed to represent the
		 *        area specified.
		 *
		 * @param area_x The width of the area
		 * @param area_z The height of the area
		 *
		 * @return The amount of cells/tiles needed to generate an area @c area_x
		 *         by @c area_z blocks.
		 */
		int32_t calcRequiredBuf(int32_t area_x, int32_t area_z);

		/**
		 * @brief Allocate the required amount of memory needed to represent the
		 *        area specified.
		 *
		 * @param size_x The width of the area
		 * @param size_z The height of the area
		 *
		 * @return A buffer adequately sized for generating a @c size_x by @c
		 *         size_z area.
		 */
		int32_t* allocCache(int32_t size_x, int32_t size_z);

		/**
		 * @brief Reset the current world seed to @c seed for all layers.
		 *
		 * @param seed Used to seed the PRNGs of all layers
		 *
		 * This function calls recursively. It will hang if there are cycles in
		 * the layer graph.
		 */
		void setWorldSeed(int64_t seed);

		/**
		 * @brief Generate the area specified.
		 *
		 * Makes a call to getMap() in order to retrieve the biome data.
		 */
		void genArea(int32_t* out, int32_t area_x, int32_t area_z, int32_t area_width, int32_t area_height);
	};

}

#endif