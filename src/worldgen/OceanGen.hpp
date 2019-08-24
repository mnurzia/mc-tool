#ifndef WORLDGEN_OCEANGEN_HPP
#define WORLDGEN_OCEANGEN_HPP

#include "JavaRandom.hpp"

/** 
 * @file OceanGen.hpp
 *
 * @brief Ocean temperature generator.
 */ 

namespace WorldGen {

	/**
	 * @brief Ocean temperature generator state.
	 *
	 * Implements an extension of the Java PRNG.
	 */
	class OceanGen {
		int32_t d[512]; /**< @brief PRNG transition table */
		double a, b, c; /**< @brief Static coordinate offsets */

	public:

		/**
		 * @brief Initialize the ocean temperature generator.
		 *
		 * Modifies and zeroes out internal state.
		 *
		 * @param seed The seed used for the Java PRNG.
		 */
		void init(int64_t seed);

		/**
		 * @brief Get the temperature of the ocean at the specified coordinates.
		 *
		 * @param d1 Coordinate 1
		 * @param d2 Coordinate 2
		 * @param d3 Coordinate 3
		 *
		 * @return The ocean temperature varying between -1 to 1.
		 */
		double getTemp(double d1, double d2, double d3);
	};

}

#endif