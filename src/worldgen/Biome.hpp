#ifndef WORLDGEN_BIOME_HPP
#define WORLDGEN_BIOME_HPP

#include <cstdint>

#include <string>
#include <vector>

#include "Feature.hpp"

/** 
 * @file Biome.hpp
 *
 * @brief Provides structures and routines pertaining to the classification and
 *        manipulation of biomes and their properties.
 */ 

namespace WorldGen::Biome {
    
    /**
     * @brief Enumeration of internal biome IDs and their names.
     */
#ifndef __DOXYGEN__
    struct BiomeID {
        typedef enum : int32_t {
#else
    enum class ID : int32_t {
#endif
            NONE                             =  -1, /**< @brief "None",                               ----t,  ----m */
            OCEAN                            =   0, /**< @brief "Ocean",                              0.50t,  0.50m */
            PLAINS                           =   1, /**< @brief "Plains",                             0.80t,  0.40m */
            DESERT                           =   2, /**< @brief "Desert",                             2.00t,  0.00m */
            MOUNTAINS                        =   3, /**< @brief "Mountains",                          0.20t,  0.30m */
            FOREST                           =   4, /**< @brief "Forest",                             0.70t,  0.80m */
            TAIGA                            =   5, /**< @brief "Taiga",                              0.25t,  0.80m */
            SWAMP                            =   6, /**< @brief "Swamp",                              0.80t,  0.90m */
            RIVER                            =   7, /**< @brief "River",                              0.50t,  0.50m */
            NETHER                           =   8, /**< @brief "Nether",                             2.00t,  0.00m */
            THE_END                          =   9, /**< @brief "The End",                            0.50t,  0.50m */
            FROZEN_OCEAN                     =  10, /**< @brief "Frozen Ocean",                       0.00t,  0.50m */
            FROZEN_RIVER                     =  11, /**< @brief "Frozen River",                       0.00t,  0.50m */
            SNOWY_TUNDRA                     =  12, /**< @brief "Snowy Tundra",                       0.00t,  0.50m */
            SNOWY_MOUNTAINS                  =  13, /**< @brief "Snowy Mountains",                    0.00t,  0.50m */
            MUSHROOM_FIELDS                  =  14, /**< @brief "Mushroom Fields",                    0.90t,  1.00m */
            MUSHROOM_FIELD_SHORE             =  15, /**< @brief "Mushroom Field Shore",               0.90t,  1.00m */
            BEACH                            =  16, /**< @brief "Beach",                              0.80t,  0.40m */
            DESERT_HILLS                     =  17, /**< @brief "Desert Hills",                       2.00t,  0.00m */
            WOODED_HILLS                     =  18, /**< @brief "Wooded Hills",                       0.70t,  0.80m */
            TAIGA_HILLS                      =  19, /**< @brief "Taiga Hills",                        0.25t,  0.80m */
            MOUNTAIN_EDGE                    =  20, /**< @brief "Mountain Edge",                      0.20t,  0.30m */
            JUNGLE                           =  21, /**< @brief "Jungle",                             0.95t,  0.90m */
            JUNGLE_HILLS                     =  22, /**< @brief "Jungle Hills",                       0.95t,  0.90m */
            JUNGLE_EDGE                      =  23, /**< @brief "Jungle Edge",                        0.95t,  0.80m */
            DEEP_OCEAN                       =  24, /**< @brief "Deep Ocean",                         0.50t,  0.50m */
            STONE_SHORE                      =  25, /**< @brief "Stone Shore",                        0.20t,  0.30m */
            SNOWY_BEACH                      =  26, /**< @brief "Snowy Beach",                        0.05t,  0.30m */
            BIRCH_FOREST                     =  27, /**< @brief "Birch Forest",                       0.60t,  0.60m */
            BIRCH_FOREST_HILLS               =  28, /**< @brief "Birch Forest Hills",                 0.60t,  0.60m */
            DARK_FOREST                      =  29, /**< @brief "Dark Forest",                        0.70t,  0.80m */
            SNOWY_TAIGA                      =  30, /**< @brief "Snowy Taiga",                       -0.50t,  0.40m */
            SNOWY_TAIGA_HILLS                =  31, /**< @brief "Snowy Taiga Hills",                 -0.50t,  0.40m */
            GIANT_TREE_TAIGA                 =  32, /**< @brief "Giant Tree Taiga",                   0.30t,  0.80m */
            GIANT_TREE_TAIGA_HILLS           =  33, /**< @brief "Giant Tree Taiga Hills",             0.30t,  0.80m */
            WOODED_MOUNTAINS                 =  34, /**< @brief "Wooded Mountains",                   0.20t,  0.30m */
            SAVANNA                          =  35, /**< @brief "Savanna",                            1.20t,  0.00m */
            SAVANNA_PLATEAU                  =  36, /**< @brief "Savanna Plateau",                    1.00t,  0.00m */
            BADLANDS                         =  37, /**< @brief "Badlands",                           2.00t,  0.00m */
            WOODED_BADLANDS_PLATEAU          =  38, /**< @brief "Wooded Badlands Plateau",            2.00t,  0.00m */
            BADLANDS_PLATEAU                 =  39, /**< @brief "Badlands Plateau",                   2.00t,  0.00m */
            SMALL_END_ISLANDS                =  40, /**< @brief "Small End Islands",                  0.50t,  0.50m */
            END_MIDLANDS                     =  41, /**< @brief "End Midlands",                       0.50t,  0.50m */
            END_HIGHLANDS                    =  42, /**< @brief "End Highlands",                      0.50t,  0.50m */
            END_BARRENS                      =  43, /**< @brief "End Barrens",                        0.50t,  0.50m */
            WARM_OCEAN                       =  44, /**< @brief "Warm Ocean",                         0.50t,  0.50m */
            LUKEWARM_OCEAN                   =  45, /**< @brief "Lukewarm Ocean",                     0.50t,  0.50m */
            COLD_OCEAN                       =  46, /**< @brief "Cold Ocean",                         0.50t,  0.50m */
            DEEP_WARM_OCEAN                  =  47, /**< @brief "Deep Warm Ocean",                    0.50t,  0.50m */
            DEEP_LUKEWARM_OCEAN              =  48, /**< @brief "Deep Lukewarm Ocean",                0.50t,  0.50m */
            DEEP_COLD_OCEAN                  =  49, /**< @brief "Deep Cold Ocean",                    0.50t,  0.50m */
            DEEP_FROZEN_OCEAN                =  50, /**< @brief "Deep Frozen Ocean",                  0.50t,  0.50m */
            THE_VOID                         = 127, /**< @brief "The Void",                           0.50t,  0.50m */
            SUNFLOWER_PLAINS                 = 129, /**< @brief "Sunflower Plains",                   0.80t,  0.40m */
            DESERT_LAKES                     = 130, /**< @brief "Desert Lakes",                       2.00t,  0.00m */
            GRAVELLY_MOUNTAINS               = 131, /**< @brief "Gravelly Mountains",                 0.20t,  0.30m */
            FLOWER_FOREST                    = 132, /**< @brief "Flower Forest",                      0.70t,  0.80m */
            TAIGA_MOUNTAINS                  = 133, /**< @brief "Taiga Mountains",                    0.25t,  0.80m */
            SWAMP_HILLS                      = 134, /**< @brief "Swamp Hills",                        0.80t,  0.90m */
            ICE_SPIKES                       = 140, /**< @brief "Ice Spikes",                         0.00t,  0.50m */
            MODIFIED_JUNGLE                  = 149, /**< @brief "Modified Jungle",                    0.95t,  0.90m */
            MODIFIED_JUNGLE_EDGE             = 151, /**< @brief "Modified Jungle Edge",               0.95t,  0.80m */
            TALL_BIRCH_FOREST                = 155, /**< @brief "Tall Birch Forest",                  0.60t,  0.60m */
            TALL_BIRCH_HILLS                 = 156, /**< @brief "Tall Birch Hills",                   0.60t,  0.60m */
            DARK_FOREST_HILLS                = 157, /**< @brief "Dark Forest Hills",                  0.70t,  0.80m */
            SNOWY_TAIGA_MOUNTAINS            = 158, /**< @brief "Snowy Taiga Mountains",             -0.50t,  0.40m */
            GIANT_SPRUCE_TAIGA               = 160, /**< @brief "Giant Spruce Taiga",                 0.25t,  0.80m */
            GIANT_SPRUCE_TAIGA_HILLS         = 161, /**< @brief "Giant Spruce Taiga Hills",           0.25t,  0.80m */
            MODIFIED_GRAVELLY_MOUNTAINS      = 162, /**< @brief "Gravelly Mountains+",                0.20t,  0.30m */
            SHATTERED_SAVANNA                = 163, /**< @brief "Shattered Savanna",                  1.10t,  0.00m */
            SHATTERED_SAVANNA_PLATEAU        = 164, /**< @brief "Shattered Savanna Plateau",          1.00t,  0.00m */
            ERODED_BADLANDS                  = 165, /**< @brief "Eroded Badlands",                    2.00t,  0.00m */
            MODIFIED_WOODED_BADLANDS_PLATEAU = 166, /**< @brief "Modified Wooded Badlands Plateau",   2.00t,  0.00m */
            MODIFIED_BADLANDS_PLATEAU        = 167, /**< @brief "Modified Badlands Plateau",          2.00t,  0.00m */
            BAMBOO_JUNGLE                    = 168, /**< @brief "Bamboo Jungle",                      0.95t,  0.90m */
            BAMBOO_JUNGLE_HILLS              = 169, /**< @brief "Bamboo Jungle Hills",                0.95t,  0.90m */
            COUNT /**< @brief Last biome number, COUNT-1 is valid but COUNT is not. */
#ifndef __DOXYGEN__
        } ID;
    };
    
    typedef BiomeID::ID ID;
#else
    };
#endif

    /**
     * @brief Enumeration of internal biome environment classes.
     */
    enum class TypeCat {
        THE_VOID        = -1, /**< @brief "The Void"        */
        OCEAN           =  0, /**< @brief "Ocean"           */
        PLAINS          =  1, /**< @brief "Plains"          */
        DESERT          =  2, /**< @brief "Desert"          */
        HILLS           =  3, /**< @brief "Hills"           */
        FOREST          =  4, /**< @brief "Forest"          */
        TAIGA           =  5, /**< @brief "Taiga"           */
        SWAMP           =  6, /**< @brief "Swamp"           */
        RIVER           =  7, /**< @brief "River"           */
        NETHER          =  8, /**< @brief "Nether"          */
        THE_END         =  9, /**< @brief "The End"         */
        SNOW            = 10, /**< @brief "Snow"            */
        MUSHROOM_ISLAND = 11, /**< @brief "Mushroom Island" */
        BEACH           = 12, /**< @brief "Beach"           */
        JUNGLE          = 13, /**< @brief "Jungle"          */
        STONE_BEACH     = 14, /**< @brief "Stone Beach"     */
        SAVANNA         = 15, /**< @brief "Savanna"         */
        BADLANDS        = 16, /**< @brief "Badlands"        */
        COUNT,
		EXTREME_HILLS   = HILLS,
		ICY				= SNOW,
		MESA			= BADLANDS,
		MUSHROOM		= MUSHROOM_ISLAND,
		THEEND			= THE_END
    };

    /**
     * @brief Enumeration of biome temperature classes.
     */
    enum class TempCat {
        THE_VOID = -1, /**< @brief "The Void" */
        OCEANIC  =  0, /**< @brief "Oceanic"  */
        WARM     =  1, /**< @brief "Warm"     */
        LUSH     =  2, /**< @brief "Lush"     */
        COLD     =  3, /**< @brief "Cold"     */
        FREEZING =  4, /**< @brief "Freezing" */  
        SPECIAL  =  5, /**< @brief "Special"  */
		ICY		 = FREEZING
    };

	/**
	 * @brief Enumeration of precipitation types.
	 */
	enum class PrecipType {
		NONE = 0,
		RAIN = 1,
		SNOW = 2
	};

	struct BiomeSpawn {
		std::string entity_name;
		std::string entity_class;
		int weight;
		int max_amt;
		int min_amt;
	};

    /**
     * @brief Biome information container.
     */
    struct Biome {
        ID      id;          /**< @brief Numeric internal ID of the biome */
        TypeCat type_cat;    /**< @brief Environment type category */
        TempCat temp_cat;    /**< @brief Temperature type category */
		ID		parent;		 /**< @brief Biome parent (unmodified biomes have none) */
        double  elevation;   /**< @brief Elevation, varies with each biome */
        double  temperature; /**< @brief Temperature, varies with each biome */
		double  scale;		 /**< @brief Terrain height scale */
		double  downfall;    /**< @brief Amount of rain? */
		/**
		 * @brief The type of precipitation in this biome 
		 */
		PrecipType precipitation;
		/**
		 * @brief The types of structures available to generate
		 */
		std::vector<Feature::Type> feature_types;
		/**
		 * @brief Bitfield of the above mapping
		 */
		int32_t feature_bits;
		/**
		 * @brief Water color.
		 */
		uint8_t water_color[3];
		/**
		 * @brief Water fog color.
		 */
		uint8_t water_fog_color[3];
		/**
		 * @brief Foliage color.
		 */
		uint8_t foliage_color[3];
		/**
		 * @brief Grass color.
		 */
		uint8_t grass_color[3];
		/**
		 * @brief Amidst color.
		 */
		uint8_t amidst_color[3];
		/**
		 * @brief All (even unimplemented) Feature types available to generate
		 */
		std::vector<std::string> all_feature_types;
		/**
		 * @brief Valid spawns
		 */
		std::vector<struct BiomeSpawn> spawns;
		/**
		 * @brief Minecraft internal ID
		 */
		std::string minecraft_id;
    };
    
    /**
     * @brief Global biome information array.
     *
     * Should never be written to under normal circumstances.
     */
    extern Biome biomes[256];

	/**
	 * @brief Global biome color array.
	 *
	 * Should never be written to under normal circumstances.
	 */
	extern uint32_t colors[256];
    
    /**
     * @brief Initialize @c biomes with values.
     *
     * Must be called before using @b any generator or other Biome function.
     */
    void initBiomes();
    
    /**
     * @brief Retrieve the type category of biome @c id.
     *
     * @return The type (environment) category of biome @c id.
     */
    static inline TypeCat getTypeCat(ID id) {
        return biomes[id & 0xff].type_cat;
    }
    
    /**
     * @brief Retrieve the temperature category of biome @c id.
     *
     * @return The temperature category of biome @c id.
     */
    static inline TempCat getTempCat(ID id) {
        return biomes[id & 0xff].temp_cat;
    }
    
    /**
     * @brief Ensure that biome @c id exists.
     *
     * @return @c true if biome @c id exists, @c false if not.
     */
    static inline bool exists(ID id) {
        return bool(!(biomes[id & 0xff].id & (~0xff)));
    }

    /**
     * @brief Test rough equality between @c id1 and @c id2.
     *
     * Determine that @c id1 and @c id2 are equal in terms of their
     * @b general environment
     *
     * @return @c true if @c id1 and @c id2 are variants of each other,
     *         @c false if not.
     */
    static inline bool equalOrPlateau(ID id1, ID id2) {
        if (id1 == id2) {
            return true;
        }
        if (id1 == ID::WOODED_BADLANDS_PLATEAU || id1 == ID::BADLANDS_PLATEAU) {
            return id2 == ID::WOODED_BADLANDS_PLATEAU || id2 == ID::BADLANDS_PLATEAU;
        }
        if (!exists(id1) || !exists(id2)) {
            return 0;
        }
        // adjust for asymmetric equality (workaround to simulate a bug in the MC java code)
        if (id1 >= 128 || id2 >= 128) {
            // skip biomes that did not overload the isEqualTo() method
            if (id2 == 130 || id2 == 133 || id2 == 134 || id2 == 149 || id2 == 151 || id2 == 155 ||
                id2 == 156 || id2 == 157 || id2 == 158 || id2 == 163 || id2 == 164) {
                return false;
            }
        }
        return getTypeCat(id1) == getTypeCat(id2);
    }
    
    /**
     * @brief Determine if @c id1 and @c id2 could be neighbors.
     *
     * @return @c true if biome @c id1 and biome @c id2 could be neighbors, @c 
     *         false if not
     */
    static inline bool canBeNeighbors(ID id1, ID id2) {
        if (equalOrPlateau(id1, id2)) {
            return 1;
        }
        if (!exists(id1) || !exists(id2)) {
            return 0;
        }
        TempCat tempCat1 = getTempCat(id1); 
        if (tempCat1 == TempCat::LUSH) {
            return 1;
        }
        TempCat tempCat2 = getTempCat(id2); 
        if (tempCat2 == TempCat::LUSH) {
            return 1;
        }
        return tempCat1 == tempCat2;
    }
    
    /**
     * @brief Determine if @c id is a shallow ocean.
     *
     * @return @c true if biome @c id is a shallow ocean, @c false if not.
     */
    static inline bool isShallowOcean(ID id) {
        const uint64_t shallow_bits =
            (1ULL << ID::OCEAN) |
            (1ULL << ID::FROZEN_OCEAN) |
            (1ULL << ID::WARM_OCEAN) |
            (1ULL << ID::LUKEWARM_OCEAN) |
            (1ULL << ID::COLD_OCEAN);
        return id < 64 && ((1ULL << id) & shallow_bits);
    }
    
    /**
     * @brief Determine if @c id is a deep ocean.
     *
     * @return @c true if biome @c id is a deep ocean, @c false if not.
     */
    static inline bool isDeepOcean(ID id) {
        const uint64_t deep_bits =
            (1ULL << ID::DEEP_OCEAN) |
            (1ULL << ID::DEEP_FROZEN_OCEAN) |
            (1ULL << ID::DEEP_WARM_OCEAN) |
            (1ULL << ID::DEEP_LUKEWARM_OCEAN) |
            (1ULL << ID::DEEP_COLD_OCEAN);
        return id < 64 && ((1ULL << id) & deep_bits);
    }

    /**
     * @brief Determine if @c id is an ocean.
     *
     * @return @c true if biome @c id is an ocean, @c false if not.
     */
    static inline bool isOcean(ID id) {
        const uint64_t ocean_bits =
            (1ULL << ID::OCEAN) |
            (1ULL << ID::FROZEN_OCEAN) |
            (1ULL << ID::WARM_OCEAN) |
            (1ULL << ID::LUKEWARM_OCEAN) |
            (1ULL << ID::COLD_OCEAN) |
            (1ULL << ID::DEEP_OCEAN) |
            (1ULL << ID::DEEP_FROZEN_OCEAN) |
            (1ULL << ID::DEEP_WARM_OCEAN) |
            (1ULL << ID::DEEP_LUKEWARM_OCEAN) |
            (1ULL << ID::DEEP_COLD_OCEAN);
        return id < 64 && ((1ULL << id) & ocean_bits);
    }
    
    /**
     * @brief Determine if @c id is snowy.
     *
     * @return @c true if biome @c id is snowy, @c false if not.
     */
    static inline bool isSnowy(ID id) {
        return exists(id) && biomes[id&0xff].temperature < 0.1;
    }
    
    /**
     * @brief Get the modified version of biome @c id
     *
     * @note This function does not check for exists() on the returned biome.
     *       Rather, it is up to the caller to determine that.
     *
     * @return @c id + 128 in Biome::ID form.
     */
    static inline ID getMod(ID id) {
        return (ID)((int32_t)id + 128);
    }

	/**
	 * @brief Check if a feature can spawn in the given biome
	 *
	 * @param id The biome to test
	 * @param type the Feature::Type to test
	 *
	 * @return @c true of @c type can occur within @c id, @c false if not.
	 */
	static inline bool canFeatureSpawn(ID id, Feature::Type type) {
		// Very fast checking because feature_bits is a bitfield; this operation
		// is O(1)
		return biomes[(int32_t)id].feature_bits & (1 << (int32_t)type);
	}
}

#endif