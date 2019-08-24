#ifndef WORLDGEN_FEATURE_HPP
#define WORLDGEN_FEATURE_HPP

#include <cstdint>

#include <string>

namespace WorldGen {
	extern const std::string feature_names[];

	class Feature {

	public:
		enum class Type {
			NONE = -1,
			MINESHAFT = 0,
			BURIED_TREASURE = 1,
			DESERT_PYRAMID = 2,
			END_CITY = 3,
			IGLOO = 4,
			JUNGLE_TEMPLE = 5,
			NETHER_BRIDGE = 6,
			OCEAN_MONUMENT = 7,
			OCEAN_RUIN = 8,
			PILLAGER_OUTPOST = 9,
			SHIPWRECK = 10,
			STRONGHOLD = 11,
			SWAMP_HUT = 12,
			VILLAGE = 13,
			WOODLAND_MANSION = 14,
			COUNT
		};

	private:
		/**
		 * @brief X coordinate.
		 */
		int32_t x;

		/**
		 * @brief Z coordinate.
		 */
		int32_t z;

		/**
		 * @brief Structure type.
		 */
		Type variant;

	public:
		/**
		 * Constructor.
		 */
		Feature(int32_t x_, int32_t z_, Type type_);

		/**
		 * @brief Get X coordinate.
		 */
		int32_t getX();

		/**
		 * @brief Get Y coordinate.
		 */
		int32_t getZ();

		/**
		 * @brief Get structure type.
		 */
		Type getType();

		/**
		 * @brief Get structure name.
		 */
		std::string getName();

		bool operator==(Feature& other);

	};

}

#endif