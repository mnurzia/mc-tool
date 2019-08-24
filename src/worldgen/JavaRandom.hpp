#ifndef WORLDGEN_JAVA_RANDOM_HPP
#define WORLDGEN_JAVA_RANDOM_HPP

#include <cstdint>

/** 
 * @file JavaRandom.hpp
 *
 * @brief Provides routines which emulate those of java.util.random.
 */ 

namespace WorldGen::JavaRandom {
    /**
     * @brief Seed the random number generator with @c seed.
     *
     * @param seed seed to be mutated
     */
    static inline void setSeed(int64_t* seed) {
        *seed = (*seed ^ 0x5deece66d) & ((1LL << 48) - 1);
    }

    /**
     * @brief Return the next pseudorandom number.
     *
     * @param seed seed to be used
     * @param bits number of bits to generate
     * @return the next pseudorandom value
     */
    static inline int32_t next(int64_t* seed, const int32_t bits) {
        *seed = (*seed * 0x5deece66d + 0xb) & ((1LL << 48) - 1);
        return (int32_t) (*seed >> (48 - bits));
    }

    /**
     * @brief Return the next pseudorandom @c int32_t.
     *
     * @param seed seed to be used
     * @param bound upper bound for output (optimized for powers of 2)
     * @return the next pseudorandom 32-bit integer between 0 and @c bound
     */
    static inline int32_t nextInt(int64_t* seed, const int32_t bound) {
        int32_t bits, val;
        const int32_t m = bound - 1;
 
        if ((m & bound) == 0) {
            return (int32_t) ((bound * (int64_t)next(seed, 31)) >> 31);
        }

        do {
            bits = next(seed, 31);
            val = bits % bound;
        } while(bits - val + m < 0);
        return val;
    }

	static inline int32_t nextInt(int64_t* seed) {
		return next(seed, 32);
	}

    /**
     * @brief Return the next pseudorandom @c int64_t.
     *
     * @param seed seed to be used
     * @return the next pseudorandom 64-bit integer
     */
    static inline int64_t nextLong(int64_t* seed) {
        return ((int64_t) next(seed, 32) << 32) + next(seed, 32);
    }

    /**
     * @brief Return the next pseudorandom @c float.
     *
     * @param seed seed to be used
     * @return the next pseudorandom floating point number
     */
    static inline float nextFloat(int64_t* seed) {
        return next(seed, 24) / (float) (1 << 24);
    }

    /**
     * @brief Return the next pseudorandom @c double.
     *
     * @param seed seed to be used
     * @return the next pseudorandom double-precision floating point number
     */
    static inline double nextDouble(int64_t* seed) {
        return (((int64_t) next(seed, 26) << 27) + next(seed, 27)) / (double) (1LL << 53);
    }

    /**
     * @brief Custom, faster alternative for the first call to nextInt(24)
     * 
     * @param seed seed to be used
     * @return the first pseudorandom 32-bit integer
     */
    static inline int32_t firstInt24(int64_t seed) {
        seed ^= 0x5deece66d;
        seed = (seed * 0x5deece66d) & 0xffffffffffff;
        seed >>= 17;
        return seed % 24;
    }

    /**
     * @brief Custom, faster alternative for the second call to nextInt(24)
     *
     * @param seed seed to be used
     * @return the second pseudorandom 32-bit integer
     */
    static inline int32_t secondInt24(int64_t seed) {
        seed ^= 0x5deece66d;
        seed = (seed * 0x5deece66d + 0xb) & 0xffffffffffff;
        seed = (seed * 0x5deece66d) & 0xffffffffffff;
        seed >>= 17;
        return seed % 24;
    }

    /**
     * @brief Jumps forwards in the random number sequence by simulating 
     *        @c amount calls to @c next().
     *
     * @param seed seed to be used
     * @param amount amount of iterations to jump forward
     */
    static inline void skipNextN(int64_t *seed, const int32_t amount) {
        for (int i = 0; i < amount; i++) {
            *seed = (*seed * 0x5deece66d + 0xb);
        }
        *seed &= 0xffffffffffff;
    }

    /**
     * @brief Returns the previous 48-bit seed which will generate
     *        @c nseed.
     *
     * The upper 16 bits are ignored, both here and in the 
     * generator.
     *
     * @param nseed desired output seed
     * @return 48-bit seed required to generate @c nseed
     */
    static inline int64_t invSeed48(int64_t nseed) {
        const int64_t x = 0x5deece66d;
        const int64_t xinv = 0xdfe05bcb1365LL;
        const int64_t y = 0xbLL;
        const int64_t m48 = 0xffffffffffffLL;

        int64_t a = nseed >> 32;
        int64_t b = nseed & 0xffffffffLL;
        
        if (b & 0x80000000LL) {
            a++;
        }

        int64_t q = ((b << 16) - y - (a << 16)*x) & m48;
        for (int64_t k = 0; k <= 5; k++) {
            int64_t d = (x - (q + (k << 48))) % x;
            d = (d + x) % x; // force the modulo and keep it positive
            if (d < 65536) {
                int64_t c = ((q + d) * xinv) & m48;
                if (c < 65536) {
                    return ((((a << 16) + c) - y) * xinv) & m48;
                }
            }
        }
        return -1;
    }

	/**
	 * @brief Mutates the seed using a special structure formula.
	 *
	 * This is used in world generation when determining where to
	 * place structures/features.
	 *
	 * @param world_seed The seed to be used.
	 * @param x The x position to feed in.
	 * @param z The z position to feed in.
	 * @return The output seed.
	 */
	static inline int64_t setStructureSeed(
		int64_t world_seed,
		int32_t x,
		int32_t z
	) {
		int64_t out = world_seed;
		setSeed(&out);
		int64_t i = nextLong(&out);
		int64_t j = nextLong(&out);
		out = x ^ i * z ^ j;
		setSeed(&out);
		return out;
	}

	/**
	 * @brief Mutates the seed using a special structure formula.
	 *
	 * This is used in world generation when determining where to
	 * place structures/features. This also uses an argument that
	 * varies per structure.
	 *
	 * @param world_seed The seed to be used.
	 * @param x The x position to feed in.
	 * @param z The z position to feed in.
	 * @param seed_modifier The unique per-structure seed modifier.
	 * @return The output seed.
	 */
	static inline int64_t setStructureSeed(
		int64_t world_seed,
		int32_t x,
		int32_t z,
		int32_t seed_modifier
	) {
		int64_t out = x * 341873128712L + z * 132897987541L + world_seed + seed_modifier;
		setSeed(&out);
		return out;
	}
}
#endif