#ifndef WORLDGEN_MAP_HPP
#define WORLDGEN_MAP_HPP

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "Biome.hpp"
#include "Layer.hpp"
#include "OceanGen.hpp"

/** 
 * @file Map.hpp
 *
 * @brief Describes functions which populate Layer's @c getMap member/method;
 *        essentially the core of world generation.
 *
 * These functions typically operate on a specific scale and are composed off
 * of each other in a top-down manner. Although the first one @b called is
 * always at the bottom (end) of the layer graph, due to the nature of these
 * functions the first one @b processed is at the top.
 */ 

namespace WorldGen::Map {
    
    /**
     * @brief Reference function that does not do any processing. 
     */
    void null(Layer* layer, int32_t* __restrict out, int32_t x, int32_t z, int32_t w, int32_t h);
    
    /**
     * @brief Skip processing; pass-through layer. Returns the exact same
     *        result into the @c out buffer as its parent would.
     */
    void skip(Layer* layer, int32_t* __restrict out, int32_t x, int32_t z, int32_t w, int32_t h);
    
    /**
     * @brief Initial island adder. Changes, at random, 10% of all cells to 
     *        Land (1), and the others to Ocean (0).
     *
     * This is the first layer function in the graph (the top function)
     */
    void island(Layer* layer, int32_t* __restrict out, int32_t area_x, int32_t area_z, int32_t area_width, int32_t area_height);

    /**
     * @brief Map zoom. Takes the output of the parent layer and creates a new
     *        map using its "zoomed" output.
     *
     * The implementation currently creates a new buffer with roughly the same
     * size as the parent one, but with a slight amount of extra space allowing
     * for neighbor checks.
     *
     * @todo Reimplement SIMD acceleration
     */
    void zoom(Layer* layer, int * __restrict out, int area_x, int area_z, int area_width, int area_height);

    /**
     * @brief Adds more island.
     *
     * Called after the initial adder, and after addSnow; this handles that 
     * case as well.
     *
     * @todo Create a variant function that only deals with post-addSnow in
     *       order to optimize both cases.
     */
    void addIsland(Layer* layer, int * __restrict out, int area_x, int area_z, int area_width, int area_height);

    /**
     * @brief Removes a large portion of the Ocean (0) biomes, changing it to
     *        Land (1).
     *
     * After this is called, the map is roughly equal parts land and ocean.
     */
    void removeTooMuchOcean(Layer* layer, int32_t * __restrict out, int32_t area_x, int32_t area_z, int32_t area_width, int32_t area_height);
    
    /**
     * @brief Gets temperature data for each cell in @c out, adding new types
     *        in the process.
     *
     * After this is called, the map contains Ocean (0), Warm (1), Cold (3),
     * and Freezing (4).
     */
    void addSnow(Layer* layer, int32_t * __restrict out, int32_t area_x, int32_t area_z, int32_t area_width, int32_t area_height);
    
    /**
     * @brief Changes certain Warm (1) lands to Lush (2).
     *
     * Warm (1) lands which are adjacent to Cold (3) or Freezing (4) change to
     * Lush (2).
     */
    void coolWarm(Layer* layer, int32_t * __restrict out, int32_t area_x, int32_t area_z, int32_t area_width, int32_t area_height);
    
    /**
     * @brief Changes certain Freezing (4) lands to Cold (3).
     *
     * Freezing (4) lands which are adjacent to Warm (1) or Lush (2) change to
     * Cold (3).
     */
    void heatIce(Layer* layer, int32_t * __restrict out, int32_t area_x, int32_t area_z, int32_t area_width, int32_t area_height);
    
    /**
     * @brief Replace certain non-ocean lands with Special lands.
     *
     * 1 in 13 non-ocean lands are marked as special, which entails adding a
     * random 4-bit number to the value shifted left by eight (* 256).
     */
    void special(Layer* layer, int32_t * __restrict out, int32_t area_x, int32_t area_z, int32_t area_width, int32_t area_height);
    
    /**
     * @brief Replace small amounts of Ocean (1) with Mushroom Fields (14).
     *
     * 1 in 100 Ocean (1) cells which are completely surrounded by more Ocean
     * are changed into Mushroom Fields (14).
     */
    void addMushroomIsland(Layer* layer, int32_t * __restrict out, int32_t area_x, int32_t area_z, int32_t area_width, int32_t area_height);
    
    /**
     * @brief Replace certain Ocean (1) with its Deep variant.
     *
     * Ocean (1) cells which are completely surrounded by more Ocean are
     * changed to their Deep variants.
     */
    void addDeepOcean(Layer* layer, int32_t * __restrict out, int32_t area_x, int32_t area_z, int32_t area_width, int32_t area_height);
    
    /**
     * @brief Change biome temperature categories to actual biome primitives.
     *
     * Introduces the following biomes: Desert, Savanna, Plains, Forest,
     * Dark Forest, Mountains, Birch Forest, Swamp, Taiga, Snowy Tundra, and
     * Snowy Taiga.
     */
    void biome(Layer* layer, int32_t * __restrict out, int32_t area_x, int32_t area_z, int32_t area_width, int32_t area_height);

    /**
     * @brief Same as @c biome(), but with Bedrock Edition compatibility.
     *
     * Lush biomes have slightly different spawning mechanics.
     */
    void biomeBE(Layer* layer, int32_t * __restrict out, int32_t area_x, int32_t area_z, int32_t area_width, int32_t area_height);
    
    /**
     * @brief Start a new branch that initializes river RNG.
     *
     * Each non-Ocean (0) cell is overwritten by a pseudorandom number between
     * 2 and 300000 (inclusive)
     */
    void riverInit(Layer* layer, int32_t * __restrict out, int32_t area_x, int32_t area_z, int32_t area_width, int32_t area_height);
    
    /**
     * @brief Add Bamboo Jungle biomes randomly.
     *
     * Each Jungle biome has a 1/10 chance of becoming a Bamboo Jungle biome.
     */
    void addBamboo(Layer* layer, int32_t * __restrict out, int32_t area_x, int32_t area_z, int32_t area_width, int32_t area_height);
    
    /**
     * @brief Replace biomes with hill and modified variants.
     *
     * Jungle Edge and Wooded Mountains biomes are added near swamp and desert,
     * respectively.
     */
    void biomeEdge(Layer* layer, int32_t * __restrict out, int32_t area_x, int32_t area_z, int32_t area_width, int32_t area_height);

    /**
     * @brief Replace biomes with their hills variant.
     *
     * The river branch is used as a pseudorandom number source. 
     */
    void hills(Layer* layer, int32_t * __restrict out, int32_t area_x, int32_t area_z, int32_t area_width, int32_t area_height);
    
    /**
     * @brief Replace biomes with their hills variant (1.13+ compatible)
     *
     * The river branch is used as a pseudorandom number source. This also
     * outputs bamboo jungle biomes.
     */
    void hills113(Layer* layer, int32_t * __restrict out, int32_t area_x, int32_t area_z, int32_t area_width, int32_t area_height);
    
    /**
     * @brief Add River biomes, using the output of the riverInit branch.
     *
     * All cells are set to River (7) or None (-1) at random.
     */
    void river(Layer* layer, int32_t * __restrict out, int32_t area_x, int32_t area_z, int32_t area_width, int32_t area_height);
    
    /**
     * @brief Smooth changes in biomes.
     *
     * Cells are "smoothed", changes in biomes are less apparent.
     */
    void smooth(Layer* layer, int32_t * __restrict out, int32_t area_x, int32_t area_z, int32_t area_width, int32_t area_height);
    
    /**
     * @brief Add Sunflower Plains (129) biomes.
     *
     * 1/57 (1.75%) of Plains (1) biomes are changed in Sunflower Plains (129).
     */
    void rareBiome(Layer* layer, int32_t * __restrict out, int32_t area_x, int32_t area_z, int32_t area_width, int32_t area_height);

    /**
     * @brief Add Shore and Beach variants of biomes.
     *
     * Generates Beach and Shore variants of biomes not completely surrounded
     * by ocean.
     */
    void shore(Layer* layer, int32_t * __restrict out, int32_t area_x, int32_t area_z, int32_t area_width, int32_t area_height);
    
    /**
     * @brief Reunite the Biome branch and the River branch, and tweak certain
     *        aquatic biomes.
     *
     * Composes the Biome and River branches and adds Frozen River (11) and
     * more Mushroom Field Shore (15)
     */
    void riverMix(Layer* layer, int32_t * __restrict out, int32_t area_x, int32_t area_z, int32_t area_width, int32_t area_height);
    
    /**
     * @brief Change Ocean biomes to temperature Ocean biomes.
     *
     * Uses the Ocean RNG to introduce 4 more ocean types, Warm, Lukewarm,
     * Frozen, and Cold.
     */
    void oceanTemp(Layer* layer, int32_t * __restrict out, int32_t area_x, int32_t area_z, int32_t area_width, int32_t area_height);
    
    /**
     * @brief Smooths changes in ocean temperatures.
     *
     * Does not use the Ocean RNG, but it might as well because it's very slow.
     * 256 samples are taken at each point when smoothing the biomes. Ouch.
     *
     * @todo optimize heavily
     */
    void oceanMix(Layer* layer, int32_t * __restrict out, int32_t area_x, int32_t area_z, int32_t area_width, int32_t area_height);
    
    /**
     * @brief Finalizes generation by zooming using a voronoi algorithm.
     *
     * The minecraft code enigmatically uses this to perform the final zoom.
     * This is likely a direct rip from MCP or something because I can't find
     * any documentation by Cubitect (not his fault, obviously - there is
     * probably no way of deciphering this) 
     */
	void voronoiZoom(Layer* layer, int32_t* __restrict out, int32_t area_x, int32_t area_z, int32_t area_width, int32_t area_height);
}

#endif