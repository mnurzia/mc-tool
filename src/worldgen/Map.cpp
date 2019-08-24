#include "Map.hpp"

namespace WorldGen::Map {

	void null(Layer* layer, int32_t* __restrict out, int32_t x, int32_t z, int32_t w, int32_t h) {
	}

	void skip(Layer* layer, int32_t* __restrict out, int32_t x, int32_t z, int32_t w, int32_t h) {
		layer->parent->getMap(layer->parent, out, x, z, w, h);
	}

	void island(Layer* layer, int32_t* __restrict out, int32_t area_x, int32_t area_z, int32_t area_width, int32_t area_height) {
		int64_t world_seed = layer->world_seed;
		int64_t seed = world_seed * (world_seed * 6364136223846793005LL + 1442695040888963407LL);

		for (int z = 0; z < area_height; z++) {
			for (int x = 0; x < area_width; x++) {
				const int64_t chunk_x = (int64_t)(x + area_x);
				const int64_t chunk_z = (int64_t)(z + area_z);
				int64_t chunk_seed = seed;
				chunk_seed += chunk_x;
				chunk_seed *= chunk_seed * 6364136223846793005LL + 1442695040888963407LL;
				chunk_seed += chunk_z;
				chunk_seed *= chunk_seed * 6364136223846793005LL + 1442695040888963407LL;
				chunk_seed += chunk_x;
				chunk_seed *= chunk_seed * 6364136223846793005LL + 1442695040888963407LL;
				chunk_seed += chunk_z;

				out[x + z * area_width] = (chunk_seed >> 24) % 10 == 0;
			}
		}

		if (area_x > -area_width && area_x <= 0 && area_z > -area_height && area_z <= 0) {
			out[-area_x + -area_z * area_width] = 1;
		}
	}

	void zoom(Layer* layer, int* __restrict out, int area_x, int area_z, int area_width, int area_height) {
		// Need an area half as large; the output of this function is four
		// times the area of the parent.
		int32_t parent_x = area_x >> 1;
		int32_t parent_z = area_z >> 1;
		int32_t parent_width = (area_width >> 1) + 2;
		int32_t parent_height = (area_height >> 1) + 2;

		layer->parent->getMap(layer->parent, out, parent_x, parent_z, parent_width, parent_height);

		int32_t new_width = (parent_width - 1) << 1;
		int32_t new_height = (parent_height - 1) << 1;
		int32_t idx, a, b;
		int32_t* buf;
		size_t amt = (new_width + 1) * (new_height + 1) * sizeof(int32_t);
		buf = (int32_t*)malloc(amt);

		if (buf == NULL) {
			printf("Unable to allocate buffer!\n");
			exit(1);
		}

		if (amt == 0) {
			printf("Size too small!\n");
			exit(1);
		}

		// Prepare the world_seed and regular seed for generation of chunks.
		const int32_t world_seed = (int32_t)layer->world_seed;
		const int32_t seed = world_seed * (world_seed * 1284865837 + 4150755663);

		for (int32_t z = 0; z < parent_height - 1; z++)
		{
			idx = (z << 1) * new_width;
			a = out[(z + 0) * parent_width];
			b = out[(z + 1) * parent_width];

			for (int32_t x = 0; x < parent_width - 1; x++)
			{
				int32_t a1 = out[x + 1 + (z + 0) * parent_width];
				int32_t b1 = out[x + 1 + (z + 1) * parent_width];

				const int32_t chunk_x = (x + parent_x) << 1;
				const int32_t chunk_z = (z + parent_z) << 1;

				// Prepare the chunk-local seed
				int32_t chunk_seed = seed;
				chunk_seed += chunk_x;
				chunk_seed *= chunk_seed * 1284865837 + 4150755663;
				chunk_seed += chunk_z;
				chunk_seed *= chunk_seed * 1284865837 + 4150755663;
				chunk_seed += chunk_x;
				chunk_seed *= chunk_seed * 1284865837 + 4150755663;
				chunk_seed += chunk_z;

				buf[idx] = a;
				buf[idx + new_width] = (chunk_seed >> 24) & 1 ? b : a;
				idx++;

				chunk_seed *= chunk_seed * 1284865837 + 4150755663;
				chunk_seed += world_seed;
				buf[idx] = (chunk_seed >> 24) & 1 ? a1 : a;

				if (layer->parent->getMap == island) {
					chunk_seed *= chunk_seed * 1284865837 + 4150755663;
					chunk_seed += world_seed;
					const int i = (chunk_seed >> 24) & 3;
					buf[idx + new_width] = i == 0 ? a : i == 1 ? a1 : i == 2 ? b : b1;
				}
				else {
					// This is more readable than an unwrapped version; kept
					// for clarity.
					if (a1 == b && b == b1) buf[idx + new_width] = a1;
					else if (a == a1 && a == b) buf[idx + new_width] = a;
					else if (a == a1 && a == b1) buf[idx + new_width] = a;
					else if (a == b && a == b1) buf[idx + new_width] = a;
					else if (a == a1 && b != b1) buf[idx + new_width] = a;
					else if (a == b && a1 != b1) buf[idx + new_width] = a;
					else if (a == b1 && a1 != b) buf[idx + new_width] = a;
					else if (a1 == b && a != b1) buf[idx + new_width] = a1;
					else if (a1 == b1 && a != b) buf[idx + new_width] = a1;
					else if (b == b1 && a != a1) buf[idx + new_width] = b;
					else {
						chunk_seed *= chunk_seed * 1284865837 + 4150755663;
						chunk_seed += world_seed;
						const int32_t i = (chunk_seed >> 24) & 3;
						buf[idx + new_width] = i == 0 ? a : i == 1 ? a1 : i == 2 ? b : b1;
					}
				}

				idx++;
				a = a1;
				b = b1;
			}
		}

		for (int32_t z = 0; z < area_height; z++) {
			memcpy(&out[z * area_width], &buf[(z + (area_z & 1)) * new_width + (area_x & 1)], area_width * sizeof(int32_t));
		}

		free(buf);
	}

	void addIsland(Layer* layer, int* __restrict out, int area_x, int area_z, int area_width, int area_height) {
		int32_t parent_x = area_x - 1;
		int32_t parent_z = area_z - 1;
		int32_t parent_width = area_width + 2;
		int32_t parent_height = area_height + 2;

		layer->parent->getMap(layer->parent, out, parent_x, parent_z, parent_width, parent_height);

		const int64_t world_seed = layer->world_seed;
		const int64_t seed = world_seed * (world_seed * 6364136223846793005LL + 1442695040888963407LL);

		for (int32_t z = 0; z < area_height; z++) {
			for (int32_t x = 0; x < area_width; x++) {
				int32_t v00 = out[x + 0 + (z + 0) * parent_width];
				int32_t v20 = out[x + 2 + (z + 0) * parent_width];
				int32_t v02 = out[x + 0 + (z + 2) * parent_width];
				int32_t v22 = out[x + 2 + (z + 2) * parent_width];
				int32_t v11 = out[x + 1 + (z + 1) * parent_width];

				if (v11 == 0 && (v00 != 0 || v20 != 0 || v02 != 0 || v22 != 0)) {
					const int64_t chunk_x = (int64_t)(x + area_x);
					const int64_t chunk_z = (int64_t)(z + area_z);
					int64_t chunk_seed = seed;
					chunk_seed += chunk_x;
					chunk_seed *= chunk_seed * 6364136223846793005LL + 1442695040888963407LL;
					chunk_seed += chunk_z;
					chunk_seed *= chunk_seed * 6364136223846793005LL + 1442695040888963407LL;
					chunk_seed += chunk_x;
					chunk_seed *= chunk_seed * 6364136223846793005LL + 1442695040888963407LL;
					chunk_seed += chunk_z;

					int32_t v = 1;
					int32_t inc = 0;

					if (v00 != 0) {
						++inc; v = v00;
						chunk_seed *= chunk_seed * 6364136223846793005LL + 1442695040888963407LL;
						chunk_seed += world_seed;
					}

					if (v20 != 0) {
						if (++inc == 1 || (chunk_seed & (1LL << 24)) == 0) {
							v = v20;
						}
						chunk_seed *= chunk_seed * 6364136223846793005LL + 1442695040888963407LL;
						chunk_seed += world_seed;
					}

					if (v02 != 0) {
						switch (++inc) {
						case 1:
							v = v02; break;
						case 2:
							if ((chunk_seed & (1LL << 24)) == 0) {
								v = v02;
							}
							break;
						default:
							if (((chunk_seed >> 24) % 3) == 0) {
								v = v02;
							}
						}

						chunk_seed *= chunk_seed * 6364136223846793005LL + 1442695040888963407LL;
						chunk_seed += world_seed;
					}

					if (v22 != 0) {
						switch (++inc) {
						case 1:
							v = v22;
							break;
						case 2:
							if ((chunk_seed & (1LL << 24)) == 0) {
								v = v22;
							}
							break;
						case 3:
							if (((chunk_seed >> 24) % 3) == 0) {
								v = v22;
							}
							break;
						default:
							if ((chunk_seed & (3LL << 24)) == 0) {
								v = v22;
							}
						}
						chunk_seed *= chunk_seed * 6364136223846793005LL + 1442695040888963407LL;
						chunk_seed += world_seed;
					}

					if ((chunk_seed >> 24) % 3 == 0) {
						out[x + z * area_width] = v;
					}
					else if (v == 4) {
						out[x + z * area_width] = 4;
					}
					else {
						out[x + z * area_width] = 0;
					}
				}
				else if (v11 > 0 && (v00 == 0 || v20 == 0 || v02 == 0 || v22 == 0)) {
					int64_t chunk_x = (int64_t)(x + area_x);
					int64_t chunk_z = (int64_t)(z + area_z);
					int64_t chunk_seed = seed;
					chunk_seed += chunk_x;
					chunk_seed *= chunk_seed * 6364136223846793005LL + 1442695040888963407LL;
					chunk_seed += chunk_z;
					chunk_seed *= chunk_seed * 6364136223846793005LL + 1442695040888963407LL;
					chunk_seed += chunk_x;
					chunk_seed *= chunk_seed * 6364136223846793005LL + 1442695040888963407LL;
					chunk_seed += chunk_z;

					if ((chunk_seed >> 24) % 5 == 0) {
						out[x + z * area_width] = (v11 == 4) ? 4 : 0;
					}
					else {
						out[x + z * area_width] = v11;
					}
				}
				else {
					out[x + z * area_width] = v11;
				}
			}
		}
	}

	void removeTooMuchOcean(Layer* layer, int32_t* __restrict out, int32_t area_x, int32_t area_z, int32_t area_width, int32_t area_height) {
		int32_t parent_x = area_x - 1;
		int32_t parent_z = area_z - 1;
		int32_t parent_width = area_width + 2;
		int32_t parent_height = area_height + 2;

		layer->parent->getMap(layer->parent, out, parent_x, parent_z, parent_width, parent_height);

		for (int32_t z = 0; z < area_height; z++) {
			for (int32_t x = 0; x < area_width; x++) {
				int32_t v11 = out[x + 1 + (z + 1) * parent_width];
				out[x + z * area_width] = v11;

				if (out[x + 1 + (z + 0) * parent_width] != 0) {
					continue;
				}
				if (out[x + 2 + (z + 1) * parent_width] != 0) {
					continue;
				}
				if (out[x + 0 + (z + 1) * parent_width] != 0) {
					continue;
				}
				if (out[x + 1 + (z + 2) * parent_width] != 0) {
					continue;
				}

				if (v11 == 0) {
					layer->setChunkSeed((int64_t)(x + area_x), (int64_t)(z + area_z));

					if (layer->mcNextInt(2) == 0) {
						out[x + z * area_width] = 1;
					}
				}
			}
		}
	}

	void addSnow(Layer* layer, int32_t* __restrict out, int32_t area_x, int32_t area_z, int32_t area_width, int32_t area_height) {
		int32_t parent_x = area_x - 1;
		int32_t parent_z = area_z - 1;
		int32_t parent_width = area_width + 2;
		int32_t parent_height = area_height + 2;

		layer->parent->getMap(layer->parent, out, parent_x, parent_z, parent_width, parent_height);

		for (int32_t z = 0; z < area_height; z++) {
			for (int32_t x = 0; x < area_width; x++) {
				int32_t v11 = out[x + 1 + (z + 1) * parent_width];

				if (Biome::isShallowOcean((Biome::ID)v11)) {
					out[x + z * area_width] = v11;
				}
				else {
					layer->setChunkSeed((int64_t)(x + area_x), (int64_t)(z + area_z));
					int32_t r = layer->mcNextInt(6);
					int32_t v;

					if (r == 0) {
						v = 4;
					}
					else if (r <= 1) {
						v = 3;
					}
					else {
						v = 1;
					}

					out[x + z * area_width] = v;
				}
			}
		}
	}

	void coolWarm(Layer* layer, int32_t* __restrict out, int32_t area_x, int32_t area_z, int32_t area_width, int32_t area_height) {
		int32_t parent_x = area_x - 1;
		int32_t parent_z = area_z - 1;
		int32_t parent_width = area_width + 2;
		int32_t parent_height = area_height + 2;

		layer->parent->getMap(layer->parent, out, parent_x, parent_z, parent_width, parent_height);

		for (int32_t z = 0; z < area_height; z++) {
			for (int32_t x = 0; x < area_width; x++) {
				int32_t v11 = out[x + 1 + (z + 1) * parent_width];

				if (v11 == 1) {
					int32_t v10 = out[x + 1 + (z + 0) * parent_width];
					int32_t v21 = out[x + 2 + (z + 1) * parent_width];
					int32_t v01 = out[x + 0 + (z + 1) * parent_width];
					int32_t v12 = out[x + 1 + (z + 2) * parent_width];

					if (v10 == 3 || v10 == 4 || v21 == 3 || v21 == 4 || v01 == 3 || v01 == 4 || v12 == 3 || v12 == 4) {
						v11 = 2;
					}
				}

				out[x + z * area_width] = v11;
			}
		}
	}

	void heatIce(Layer* layer, int32_t* __restrict out, int32_t area_x, int32_t area_z, int32_t area_width, int32_t area_height) {
		int32_t parent_x = area_x - 1;
		int32_t parent_z = area_z - 1;
		int32_t parent_width = area_width + 2;
		int32_t parent_height = area_height + 2;

		layer->parent->getMap(layer->parent, out, parent_x, parent_z, parent_width, parent_height);

		for (int32_t z = 0; z < area_height; z++) {
			for (int32_t x = 0; x < area_width; x++) {
				int32_t v11 = out[x + 1 + (z + 1) * parent_width];

				if (v11 == 4) {
					int32_t v10 = out[x + 1 + (z + 0) * parent_width];
					int32_t v21 = out[x + 2 + (z + 1) * parent_width];
					int32_t v01 = out[x + 0 + (z + 1) * parent_width];
					int32_t v12 = out[x + 1 + (z + 2) * parent_width];

					if (v10 == 1 || v10 == 2 || v21 == 1 || v21 == 2 || v01 == 1 || v01 == 2 || v12 == 1 || v12 == 2) {
						v11 = 3;
					}
				}

				out[x + z * area_width] = v11;
			}
		}
	}

	void special(Layer* layer, int32_t* __restrict out, int32_t area_x, int32_t area_z, int32_t area_width, int32_t area_height) {
		// note that this layer does not need neighbor inspection; thus the 
		// parent layer can be queried as-is
		layer->parent->getMap(layer->parent, out, area_x, area_z, area_width, area_height);

		for (int32_t z = 0; z < area_height; z++) {
			for (int32_t x = 0; x < area_width; x++) {
				int32_t v = out[x + z * area_width];
				if (v == 0) {
					continue;
				}

				layer->setChunkSeed((int64_t)(x + area_x), (int64_t)(z + area_z));

				if (layer->mcNextInt(13) == 0) {
					v |= ((1 + layer->mcNextInt(15)) << 8) & 0xf00;
					// 1 to 1 mapping so 'out' can be overwritten immediately
					out[x + z * area_width] = v;
				}
			}
		}
	}

	void addMushroomIsland(Layer* layer, int32_t* __restrict out, int32_t area_x, int32_t area_z, int32_t area_width, int32_t area_height) {
		int32_t parent_x = area_x - 1;
		int32_t parent_z = area_z - 1;
		int32_t parent_width = area_width + 2;
		int32_t parent_height = area_height + 2;

		layer->parent->getMap(layer->parent, out, parent_x, parent_z, parent_width, parent_height);

		for (int32_t z = 0; z < area_height; z++) {
			for (int32_t x = 0; x < area_width; x++) {
				int32_t v11 = out[x + 1 + (z + 1) * parent_width];

				// surrounded by ocean?
				if (
					(v11 == 0) &&
					(!out[x + 0 + (z + 0) * parent_width]) &&
					(!out[x + 2 + (z + 0) * parent_width]) &&
					(!out[x + 0 + (z + 2) * parent_width]) &&
					(!out[x + 2 + (z + 2) * parent_width])) {
					layer->setChunkSeed((int64_t)(x + area_x), (int64_t)(z + area_z));
					if (layer->mcNextInt(100) == 0) {
						out[x + z * area_width] = Biome::ID::MUSHROOM_FIELDS;
						continue;
					}
				}

				out[x + z * area_width] = v11;
			}
		}
	}

	void addDeepOcean(Layer* layer, int32_t* __restrict out, int32_t area_x, int32_t area_z, int32_t area_width, int32_t area_height) {
		int32_t parent_x = area_x - 1;
		int32_t parent_z = area_z - 1;
		int32_t parent_width = area_width + 2;
		int32_t parent_height = area_height + 2;

		layer->parent->getMap(layer->parent, out, parent_x, parent_z, parent_width, parent_height);

		for (int32_t z = 0; z < area_height; z++) {
			for (int32_t x = 0; x < area_width; x++) {
				int32_t v11 = out[(x + 1) + (z + 1) * parent_width];

				if (Biome::isShallowOcean((Biome::ID)v11)) {
					// count adjacent oceans
					int8_t oceans = 0;
					if (Biome::isShallowOcean((Biome::ID)out[(x + 1) + (z + 0) * parent_width])) {
						oceans++;
					}
					if (Biome::isShallowOcean((Biome::ID)out[(x + 2) + (z + 1) * parent_width])) {
						oceans++;
					}
					if (Biome::isShallowOcean((Biome::ID)out[(x + 0) + (z + 1) * parent_width])) {
						oceans++;
					}
					if (Biome::isShallowOcean((Biome::ID)out[(x + 1) + (z + 2) * parent_width])) {
						oceans++;
					}

					if (oceans > 3) {
						switch (v11) {
						case Biome::ID::WARM_OCEAN:
							v11 = Biome::ID::DEEP_WARM_OCEAN;
							break;
						case Biome::ID::LUKEWARM_OCEAN:
							v11 = Biome::ID::DEEP_LUKEWARM_OCEAN;
							break;
						case Biome::ID::OCEAN:
							v11 = Biome::ID::DEEP_OCEAN;
							break;
						case Biome::ID::COLD_OCEAN:
							v11 = Biome::ID::DEEP_COLD_OCEAN;
							break;
						case Biome::ID::FROZEN_OCEAN:
							v11 = Biome::ID::DEEP_FROZEN_OCEAN;
							break;
						default:
							v11 = Biome::ID::DEEP_OCEAN;
						}
					}
				}

				out[x + z * area_width] = v11;
			}
		}
	}

	static const Biome::ID warm_biomes[] = {
		Biome::ID::DESERT,
		Biome::ID::DESERT,
		Biome::ID::DESERT,
		Biome::ID::SAVANNA,
		Biome::ID::SAVANNA,
		Biome::ID::PLAINS
	};

	static const Biome::ID lush_biomes[] = {
		Biome::ID::FOREST,
		Biome::ID::DARK_FOREST,
		Biome::ID::MOUNTAINS,
		Biome::ID::PLAINS,
		Biome::ID::BIRCH_FOREST,
		Biome::ID::SWAMP
	};

	static const Biome::ID cold_biomes[] = {
		Biome::ID::FOREST,
		Biome::ID::MOUNTAINS,
		Biome::ID::TAIGA,
		Biome::ID::PLAINS
	};

	static const Biome::ID snow_biomes[] = {
		Biome::ID::SNOWY_TUNDRA,
		Biome::ID::SNOWY_TUNDRA,
		Biome::ID::SNOWY_TUNDRA,
		Biome::ID::SNOWY_TAIGA
	};

	static const Biome::ID lush_biomes_be[] = {
		Biome::ID::FOREST,
		Biome::ID::DARK_FOREST,
		Biome::ID::MOUNTAINS,
		Biome::ID::PLAINS,
		Biome::ID::PLAINS,
		Biome::ID::PLAINS,
		Biome::ID::BIRCH_FOREST,
		Biome::ID::SWAMP
	};

	void biome(Layer* layer, int32_t* __restrict out, int32_t area_x, int32_t area_z, int32_t area_width, int32_t area_height) {
		layer->parent->getMap(layer->parent, out, area_x, area_z, area_width, area_height);
		for (int32_t z = 0; z < area_height; z++) {
			for (int32_t x = 0; x < area_width; x++) {
				int32_t idx = x + z * area_width;
				int32_t id = out[idx];
				int32_t has_high_bit = (id & 0xf00) >> 8;
				id &= -0xf01;

				if (
					Biome::getTypeCat((Biome::ID)id) == Biome::TypeCat::OCEAN ||
					id == Biome::ID::MUSHROOM_FIELDS
					) {
					out[idx] = id;
					continue;
				}

				layer->setChunkSeed((int64_t)(x + area_x), (int64_t)(z + area_z));

				switch (static_cast<Biome::TempCat>(id)) {
				case Biome::TempCat::WARM:
					if (has_high_bit) {
						out[idx] = (layer->mcNextInt(3) == 0) ?
							Biome::ID::BADLANDS_PLATEAU :
							Biome::ID::WOODED_BADLANDS_PLATEAU;
					}
					else {
						out[idx] = warm_biomes[layer->mcNextInt(6)];
					}
					break;
				case Biome::TempCat::LUSH:
					if (has_high_bit) {
						out[idx] = Biome::ID::JUNGLE;
					}
					else {
						out[idx] = lush_biomes[layer->mcNextInt(6)];
					}
					break;
				case Biome::TempCat::COLD:
					if (has_high_bit) {
						out[idx] = Biome::ID::GIANT_TREE_TAIGA;
					}
					else {
						out[idx] = cold_biomes[layer->mcNextInt(4)];
					}
					break;
				case Biome::TempCat::FREEZING:
					out[idx] = snow_biomes[layer->mcNextInt(4)];
					break;
				default:
					out[idx] = Biome::ID::MUSHROOM_FIELDS;
				}
			}
		}
	}

	void biomeBE(Layer* layer, int32_t* __restrict out, int32_t area_x, int32_t area_z, int32_t area_width, int32_t area_height) {
		layer->parent->getMap(layer->parent, out, area_x, area_z, area_width, area_height);

		for (int32_t z = 0; z < area_height; z++) {
			for (int32_t x = 0; x < area_width; x++) {
				int32_t idx = x + z * area_width;
				int32_t id = out[idx];
				int32_t has_high_bit = (id & 0xf00) >> 8;
				id &= -0xf01;

				if (
					Biome::getTypeCat((Biome::ID)id) == Biome::TypeCat::OCEAN ||
					id == Biome::ID::MUSHROOM_FIELDS
					) {
					out[idx] = id;
					continue;
				}

				layer->setChunkSeed((int64_t)(x + area_x), (int64_t)(z + area_z));

				switch (static_cast<Biome::TempCat>(id)) {
				case Biome::TempCat::WARM:
					if (has_high_bit) {
						out[idx] = (layer->mcNextInt(3) == 0) ?
							Biome::ID::BADLANDS_PLATEAU :
							Biome::ID::WOODED_BADLANDS_PLATEAU;
					}
					else {
						out[idx] = warm_biomes[layer->mcNextInt(6)];
					}
					break;
				case Biome::TempCat::LUSH:
					if (has_high_bit) {
						out[idx] = Biome::ID::JUNGLE;
					}
					else {
						out[idx] = lush_biomes_be[layer->mcNextInt(6)];
					}
					break;
				case Biome::TempCat::COLD:
					if (has_high_bit) {
						out[idx] = Biome::ID::GIANT_TREE_TAIGA;
					}
					else {
						out[idx] = cold_biomes[layer->mcNextInt(4)];
					}
					break;
				case Biome::TempCat::FREEZING:
					out[idx] = snow_biomes[layer->mcNextInt(4)];
					break;
				default:
					out[idx] = Biome::ID::MUSHROOM_FIELDS;
				}
			}
		}
	}

	void riverInit(Layer* layer, int32_t* __restrict out, int32_t area_x, int32_t area_z, int32_t area_width, int32_t area_height) {
		layer->parent->getMap(layer->parent, out, area_x, area_z, area_width, area_height);

		for (int32_t z = 0; z < area_height; z++) {
			for (int32_t x = 0; x < area_width; x++) {
				if (out[x + z * area_width] > 0) {
					layer->setChunkSeed((int64_t)(x + area_x), (int64_t)(z + area_z));
					out[x + z * area_width] = layer->mcNextInt(299999) + 2;
				}
				else {
					out[x + z * area_width] = 0;
				}
			}
		}
	}

	void addBamboo(Layer* layer, int32_t* __restrict out, int32_t area_x, int32_t area_z, int32_t area_width, int32_t area_height) {
		layer->parent->getMap(layer->parent, out, area_x, area_z, area_width, area_height);

		for (int32_t z = 0; z < area_height; z++) {
			for (int32_t x = 0; x < area_width; x++) {
				int idx = x + z * area_width;
				if (out[idx] != Biome::ID::JUNGLE) {
					continue;
				}

				layer->setChunkSeed((int64_t)(x + area_x), (int64_t)(z + area_z));
				if (layer->mcNextInt(10) == 0) {
					out[idx] = Biome::ID::BAMBOO_JUNGLE;
				}
			}
		}
	}

	static inline bool replaceEdge(int32_t* out, int32_t idx, int32_t v10, int32_t v21, int32_t v01, int32_t v12, int32_t id, Biome::ID base_id, Biome::ID edge_id) {
		if (id != base_id) {
			return false;
		}

		if (
			Biome::equalOrPlateau((Biome::ID)v10, base_id) &&
			Biome::equalOrPlateau((Biome::ID)v21, base_id) &&
			Biome::equalOrPlateau((Biome::ID)v01, base_id) &&
			Biome::equalOrPlateau((Biome::ID)v12, base_id)
			) {
			out[idx] = id;
		}
		else {
			out[idx] = edge_id;
		}

		return true;
	}

	void biomeEdge(Layer* layer, int32_t* __restrict out, int32_t area_x, int32_t area_z, int32_t area_width, int32_t area_height) {
		int32_t parent_x = area_x - 1;
		int32_t parent_z = area_z - 1;
		int32_t parent_width = area_width + 2;
		int32_t parent_height = area_height + 2;

		layer->parent->getMap(layer->parent, out, parent_x, parent_z, parent_width, parent_height);

		for (int32_t z = 0; z < area_height; z++) {
			for (int32_t x = 0; x < area_width; x++) {
				int32_t v11 = out[x + 1 + (z + 1) * parent_width];

				int32_t v10 = out[x + 1 + (z + 0) * parent_width];
				int32_t v21 = out[x + 2 + (z + 1) * parent_width];
				int32_t v01 = out[x + 0 + (z + 1) * parent_width];
				int32_t v12 = out[x + 1 + (z + 2) * parent_width];

				if (/*!replaceEdgeIfNecessary(out, x + z*area_width, v10, v21, v01, v12, v11, mountains, mountain_edge) &&*/
					!replaceEdge(out, x + z * area_width, v10, v21, v01, v12, v11, Biome::ID::WOODED_BADLANDS_PLATEAU, Biome::ID::BADLANDS) &&
					!replaceEdge(out, x + z * area_width, v10, v21, v01, v12, v11, Biome::ID::BADLANDS_PLATEAU, Biome::ID::BADLANDS) &&
					!replaceEdge(out, x + z * area_width, v10, v21, v01, v12, v11, Biome::ID::GIANT_TREE_TAIGA, Biome::ID::TAIGA)
					) {
					if (v11 == Biome::ID::DESERT) {
						if (
							v10 != Biome::ID::SNOWY_TUNDRA &&
							v21 != Biome::ID::SNOWY_TUNDRA &&
							v01 != Biome::ID::SNOWY_TUNDRA &&
							v12 != Biome::ID::SNOWY_TUNDRA
							) {
							out[x + z * area_width] = v11;
						}
						else {
							out[x + z * area_width] = Biome::ID::WOODED_MOUNTAINS;
						}
					}
					else if (v11 == Biome::ID::SWAMP) {
						if (
							v10 != Biome::ID::DESERT &&
							v21 != Biome::ID::DESERT &&
							v01 != Biome::ID::DESERT &&
							v12 != Biome::ID::DESERT &&
							v10 != Biome::ID::SNOWY_TAIGA &&
							v21 != Biome::ID::SNOWY_TAIGA &&
							v01 != Biome::ID::SNOWY_TAIGA &&
							v12 != Biome::ID::SNOWY_TAIGA &&
							v10 != Biome::ID::SNOWY_TUNDRA &&
							v21 != Biome::ID::SNOWY_TUNDRA &&
							v01 != Biome::ID::SNOWY_TUNDRA &&
							v12 != Biome::ID::SNOWY_TUNDRA
							) {
							if (
								v10 != Biome::ID::JUNGLE &&
								v12 != Biome::ID::JUNGLE &&
								v21 != Biome::ID::JUNGLE &&
								v01 != Biome::ID::JUNGLE &&
								v10 != Biome::ID::BAMBOO_JUNGLE &&
								v12 != Biome::ID::BAMBOO_JUNGLE &&
								v21 != Biome::ID::BAMBOO_JUNGLE &&
								v01 != Biome::ID::BAMBOO_JUNGLE
								) {
								out[x + z * area_width] = v11;
							}
							else {
								out[x + z * area_width] = Biome::ID::JUNGLE_EDGE;
							}
						}
						else {
							out[x + z * area_width] = Biome::ID::PLAINS;
						}
					}
					else {
						out[x + z * area_width] = v11;
					}
				}
			}
		}
	}

	void hills(Layer* layer, int32_t* __restrict out, int32_t area_x, int32_t area_z, int32_t area_width, int32_t area_height) {
		int32_t parent_x = area_x - 1;
		int32_t parent_z = area_z - 1;
		int32_t parent_width = area_width + 2;
		int32_t parent_height = area_height + 2;

		if (layer->parent_2 == NULL) {
			printf("hills() requires two parents! Use setupMultiLayer()\n");
			exit(1);
		}

		int32_t* buf = (int32_t*)malloc(parent_width * parent_height * sizeof(int32_t));

		layer->parent->getMap(layer->parent, out, parent_x, parent_z, parent_width, parent_height);
		memcpy(buf, out, parent_width * parent_height * sizeof(int32_t));

		layer->parent_2->getMap(layer->parent_2, out, parent_x, parent_z, parent_width, parent_height);

		for (int32_t z = 0; z < area_height; z++) {
			for (int32_t x = 0; x < area_width; x++) {
				layer->setChunkSeed((int64_t)(x + area_x), (int64_t)(z + area_z));
				int32_t a11 = buf[x + 1 + (z + 1) * parent_width]; // biome branch
				int32_t b11 = out[x + 1 + (z + 1) * parent_width]; // river branch
				int32_t idx = x + z * area_width;

				int32_t var12 = (b11 - 2) % 29 == 0;

				if (a11 != 0 && b11 >= 2 && (b11 - 2) % 29 == 1 && a11 < 128) {
					out[idx] = (Biome::exists(Biome::getMod((Biome::ID)a11)) ? a11 + 128 : a11);
				}
				else if (layer->mcNextInt(3) != 0 && !var12) {
					out[idx] = a11;
				}
				else {
					Biome::ID hill_id = (Biome::ID)a11;

					switch (a11) {
					case Biome::ID::DESERT:
						hill_id = Biome::ID::DESERT_HILLS; break;
					case Biome::ID::FOREST:
						hill_id = Biome::ID::WOODED_HILLS; break;
					case Biome::ID::BIRCH_FOREST:
						hill_id = Biome::ID::BIRCH_FOREST_HILLS; break;
					case Biome::ID::DARK_FOREST:
						hill_id = Biome::ID::PLAINS; break;
					case Biome::ID::TAIGA:
						hill_id = Biome::ID::TAIGA_HILLS; break;
					case Biome::ID::GIANT_TREE_TAIGA:
						hill_id = Biome::ID::GIANT_TREE_TAIGA_HILLS; break;
					case Biome::ID::SNOWY_TAIGA:
						hill_id = Biome::ID::SNOWY_TAIGA_HILLS; break;
					case Biome::ID::PLAINS:
						hill_id = (layer->mcNextInt(3) == 0) ?
							Biome::ID::WOODED_HILLS :
							Biome::ID::FOREST;
						break;
					case Biome::ID::SNOWY_TUNDRA:
						hill_id = Biome::ID::SNOWY_MOUNTAINS; break;
					case Biome::ID::JUNGLE:
						hill_id = Biome::ID::JUNGLE_HILLS; break;
					case Biome::ID::OCEAN:
						hill_id = Biome::ID::DEEP_OCEAN; break;
					case Biome::ID::MOUNTAINS:
						hill_id = Biome::ID::WOODED_MOUNTAINS; break;
					case Biome::ID::SAVANNA:
						hill_id = Biome::ID::SAVANNA_PLATEAU; break;
					default:
						if (Biome::equalOrPlateau((Biome::ID)a11, Biome::ID::WOODED_BADLANDS_PLATEAU)) {
							hill_id = Biome::ID::BADLANDS;
						}
						else if (a11 == Biome::ID::DEEP_OCEAN && layer->mcNextInt(3) == 0) {
							hill_id = (layer->mcNextInt(2) == 0) ?
								Biome::ID::PLAINS :
								Biome::ID::FOREST;
						}
						break;
					}

					if (var12 && hill_id != a11) {
						if (Biome::exists(Biome::getMod(hill_id))) {
							hill_id = Biome::getMod(hill_id);
						}
						else {
							hill_id = (Biome::ID)a11;
						}
					}

					if (hill_id == a11) {
						out[idx] = a11;
					}
					else {
						Biome::ID a10 = (Biome::ID)buf[x + 1 + (z + 0) * parent_width];
						Biome::ID a21 = (Biome::ID)buf[x + 2 + (z + 1) * parent_width];
						Biome::ID a01 = (Biome::ID)buf[x + 0 + (z + 1) * parent_width];
						Biome::ID a12 = (Biome::ID)buf[x + 1 + (z + 2) * parent_width];
						Biome::ID a11_ = (Biome::ID)a11;
						int8_t equals = 0;

						if (Biome::equalOrPlateau(a10, a11_)) equals++;
						if (Biome::equalOrPlateau(a21, a11_)) equals++;
						if (Biome::equalOrPlateau(a01, a11_)) equals++;
						if (Biome::equalOrPlateau(a12, a11_)) equals++;

						if (equals >= 3) {
							out[idx] = hill_id;
						}
						else {
							out[idx] = a11;
						}
					}
				}
			}
		}

		free(buf);
	}

	void hills113(Layer* layer, int32_t* __restrict out, int32_t area_x, int32_t area_z, int32_t area_width, int32_t area_height) {
		int32_t parent_x = area_x - 1;
		int32_t parent_z = area_z - 1;
		int32_t parent_width = area_width + 2;
		int32_t parent_height = area_height + 2;

		if (layer->parent_2 == NULL) {
			printf("hills113() requires two parents! Use setupMultiLayer()\n");
			exit(1);
		}

		int32_t* buf = (int32_t*)malloc(parent_width * parent_height * sizeof(int32_t));

		layer->parent->getMap(layer->parent, out, parent_x, parent_z, parent_width, parent_height);
		memcpy(buf, out, parent_width * parent_height * sizeof(int32_t));

		layer->parent_2->getMap(layer->parent_2, out, parent_x, parent_z, parent_width, parent_height);

		for (int32_t z = 0; z < area_height; z++) {
			for (int32_t x = 0; x < area_width; x++) {
				layer->setChunkSeed((int64_t)(x + area_x), (int64_t)(z + area_z));
				int32_t a11 = buf[x + 1 + (z + 1) * parent_width]; // biome branch
				int32_t b11 = out[x + 1 + (z + 1) * parent_width]; // river branch
				int32_t idx = x + z * area_width;

				int32_t bn = (b11 - 2) % 29;
				if (!(Biome::isOcean((Biome::ID)a11) || b11 < 2 || bn != 1 || a11 >= 128)) {
					out[idx] = (Biome::exists(Biome::getMod((Biome::ID)a11)) ? a11 + 128 : a11);
				}
				else if (layer->mcNextInt(3) == 0 || bn == 0) {
					Biome::ID hill_id = (Biome::ID)a11;

					switch (a11) {
					case Biome::ID::DESERT:
						hill_id = Biome::ID::DESERT_HILLS; break;
					case Biome::ID::FOREST:
						hill_id = Biome::ID::WOODED_HILLS; break;
					case Biome::ID::BIRCH_FOREST:
						hill_id = Biome::ID::BIRCH_FOREST_HILLS; break;
					case Biome::ID::DARK_FOREST:
						hill_id = Biome::ID::PLAINS; break;
					case Biome::ID::TAIGA:
						hill_id = Biome::ID::TAIGA_HILLS; break;
					case Biome::ID::GIANT_TREE_TAIGA:
						hill_id = Biome::ID::GIANT_TREE_TAIGA_HILLS; break;
					case Biome::ID::SNOWY_TAIGA:
						hill_id = Biome::ID::SNOWY_TAIGA_HILLS; break;
					case Biome::ID::PLAINS:
						hill_id = (layer->mcNextInt(3) == 0) ?
							Biome::ID::WOODED_HILLS :
							Biome::ID::FOREST;
						break;
					case Biome::ID::SNOWY_TUNDRA:
						hill_id = Biome::ID::SNOWY_MOUNTAINS; break;
					case Biome::ID::JUNGLE:
						hill_id = Biome::ID::JUNGLE_HILLS; break;
					case Biome::ID::BAMBOO_JUNGLE:
						hill_id = Biome::ID::BAMBOO_JUNGLE_HILLS; break;
					case Biome::ID::OCEAN:
						hill_id = Biome::ID::DEEP_OCEAN; break;
					case Biome::ID::MOUNTAINS:
						hill_id = Biome::ID::WOODED_MOUNTAINS; break;
					case Biome::ID::SAVANNA:
						hill_id = Biome::ID::SAVANNA_PLATEAU; break;
					default:
						if (Biome::equalOrPlateau((Biome::ID)a11, Biome::ID::WOODED_BADLANDS_PLATEAU)) {
							hill_id = Biome::ID::BADLANDS;
						}
						else if (isDeepOcean((Biome::ID)a11) && layer->mcNextInt(3) == 0) {
							hill_id = (layer->mcNextInt(2) == 0) ?
								Biome::ID::PLAINS :
								Biome::ID::FOREST;
						}
						break;
					}

					if (bn == 0 && hill_id != a11) {
						if (Biome::exists(Biome::getMod(hill_id))) {
							hill_id = Biome::getMod(hill_id);
						}
						else {
							hill_id = (Biome::ID)a11;
						}
					}

					if (hill_id != a11) {
						Biome::ID a10 = (Biome::ID)buf[x + 1 + (z + 0) * parent_width];
						Biome::ID a21 = (Biome::ID)buf[x + 2 + (z + 1) * parent_width];
						Biome::ID a01 = (Biome::ID)buf[x + 0 + (z + 1) * parent_width];
						Biome::ID a12 = (Biome::ID)buf[x + 1 + (z + 2) * parent_width];
						Biome::ID a11_ = (Biome::ID)a11;
						int8_t equals = 0;

						if (Biome::equalOrPlateau(a10, a11_)) equals++;
						if (Biome::equalOrPlateau(a21, a11_)) equals++;
						if (Biome::equalOrPlateau(a01, a11_)) equals++;
						if (Biome::equalOrPlateau(a12, a11_)) equals++;

						if (equals >= 3) {
							out[idx] = hill_id;
						}
						else {
							out[idx] = a11;
						}
					}
					else {
						out[idx] = a11;
					}
				}
				else {
					out[idx] = a11;
				}
			}
		}

		free(buf);
	}

	static inline int32_t reduceID(int32_t id) {
		return id >= 2 ? 2 + (id & 1) : id;
	}

	void river(Layer* layer, int32_t* __restrict out, int32_t area_x, int32_t area_z, int32_t area_width, int32_t area_height) {
		int32_t parent_x = area_x - 1;
		int32_t parent_z = area_z - 1;
		int32_t parent_width = area_width + 2;
		int32_t parent_height = area_height + 2;

		layer->parent->getMap(layer->parent, out, parent_x, parent_z, parent_width, parent_height);

		for (int32_t z = 0; z < area_height; z++) {
			for (int32_t x = 0; x < area_width; x++) {
				int32_t v01 = reduceID(out[x + 0 + (z + 1) * parent_width]);
				int32_t v21 = reduceID(out[x + 2 + (z + 1) * parent_width]);
				int32_t v10 = reduceID(out[x + 1 + (z + 0) * parent_width]);
				int32_t v12 = reduceID(out[x + 1 + (z + 2) * parent_width]);
				int32_t v11 = reduceID(out[x + 1 + (z + 1) * parent_width]);

				if (v11 == v01 && v11 == v10 && v11 == v21 && v11 == v12) {
					out[x + z * area_width] = -1;
				}
				else {
					out[x + z * area_width] = Biome::ID::RIVER;
				}
			}
		}
	}

	void smooth(Layer* layer, int32_t* __restrict out, int32_t area_x, int32_t area_z, int32_t area_width, int32_t area_height) {
		int32_t parent_x = area_x - 1;
		int32_t parent_z = area_z - 1;
		int32_t parent_width = area_width + 2;
		int32_t parent_height = area_height + 2;

		layer->parent->getMap(layer->parent, out, parent_x, parent_z, parent_width, parent_height);

		for (int32_t z = 0; z < area_height; z++) {
			for (int32_t x = 0; x < area_width; x++) {
				int32_t v11 = out[x + 1 + (z + 1) * parent_width];
				int32_t v10 = out[x + 1 + (z + 0) * parent_width];
				int32_t v21 = out[x + 2 + (z + 1) * parent_width];
				int32_t v01 = out[x + 0 + (z + 1) * parent_width];
				int32_t v12 = out[x + 1 + (z + 2) * parent_width];

				if (v01 == v21 && v10 == v12) {
					layer->setChunkSeed((int64_t)(x + area_x), (int64_t)(z + area_z));

					if (layer->mcNextInt(2) == 0) {
						v11 = v01;
					}
					else {
						v11 = v10;
					}
				}
				else {
					if (v01 == v21) {
						v11 = v01;
					}
					if (v10 == v12) {
						v11 = v10;
					}
				}

				out[x + z * area_width] = v11;
			}
		}
	}

	void rareBiome(Layer* layer, int32_t* __restrict out, int32_t area_x, int32_t area_z, int32_t area_width, int32_t area_height) {
		int32_t parent_x = area_x - 1;
		int32_t parent_z = area_z - 1;
		int32_t parent_width = area_width + 2;
		int32_t parent_height = area_height + 2;

		layer->parent->getMap(layer->parent, out, parent_x, parent_z, parent_width, parent_height);

		for (int32_t z = 0; z < area_height; z++) {
			for (int32_t x = 0; x < area_width; x++) {
				layer->setChunkSeed((int64_t)(x + area_x), (int64_t)(z + area_z));
				int32_t v11 = out[x + 1 + (z + 1) * parent_width];

				if (layer->mcNextInt(57) == 0 && v11 == Biome::ID::PLAINS) {
					// Sunflower Plains
					out[x + z * area_width] = Biome::ID::SUNFLOWER_PLAINS;
				}
				else {
					out[x + z * area_width] = v11;
				}
			}
		}
	}

	inline static bool replaceOcean(int32_t* out, int32_t idx, int32_t v10, int32_t v21, int32_t v01, int32_t v12, int32_t id, Biome::ID replace_id) {
		if (Biome::isOcean((Biome::ID)id)) {
			return 0;
		}

		if (
			!Biome::isOcean((Biome::ID)v10) &&
			!Biome::isOcean((Biome::ID)v21) &&
			!Biome::isOcean((Biome::ID)v01) &&
			!Biome::isOcean((Biome::ID)v12)
			) {
			out[idx] = id;
		}
		else {
			out[idx] = replace_id;
		}

		return 1;
	}

	inline static bool isBiomeJFTO(Biome::ID id) {
		return Biome::exists(id) &&
			(
				Biome::getTypeCat(id) == Biome::TypeCat::JUNGLE ||
				id == Biome::ID::FOREST ||
				id == Biome::ID::TAIGA ||
				Biome::isOcean(id)
				);
	}

	void shore(Layer* layer, int32_t* __restrict out, int32_t area_x, int32_t area_z, int32_t area_width, int32_t area_height) {
		int32_t parent_x = area_x - 1;
		int32_t parent_z = area_z - 1;
		int32_t parent_width = area_width + 2;
		int32_t parent_height = area_height + 2;

		layer->parent->getMap(layer->parent, out, parent_x, parent_z, parent_width, parent_height);

		for (int32_t z = 0; z < area_height; z++) {
			for (int32_t x = 0; x < area_width; x++) {
				int32_t v11 = out[x + 1 + (z + 1) * parent_width];
				int32_t v10 = out[x + 1 + (z + 0) * parent_width];
				int32_t v21 = out[x + 2 + (z + 1) * parent_width];
				int32_t v01 = out[x + 0 + (z + 1) * parent_width];
				int32_t v12 = out[x + 1 + (z + 2) * parent_width];

				int32_t biome = Biome::exists((Biome::ID)v11) ? v11 : 0;

				if (v11 == Biome::ID::MUSHROOM_FIELDS) {
					if (
						v10 != Biome::ID::OCEAN &&
						v21 != Biome::ID::OCEAN &&
						v01 != Biome::ID::OCEAN &&
						v12 != Biome::ID::OCEAN
						) {
						out[x + z * area_width] = v11;
					}
					else {
						out[x + z * area_width] = Biome::ID::MUSHROOM_FIELD_SHORE;
					}
				}
				else if (Biome::getTypeCat((Biome::ID)biome) == Biome::TypeCat::JUNGLE) {
					if (
						isBiomeJFTO((Biome::ID)v10) &&
						isBiomeJFTO((Biome::ID)v21) &&
						isBiomeJFTO((Biome::ID)v01) &&
						isBiomeJFTO((Biome::ID)v12)
						) {
						if (
							!Biome::isOcean((Biome::ID)v10) &&
							!Biome::isOcean((Biome::ID)v21) &&
							!Biome::isOcean((Biome::ID)v01) &&
							!Biome::isOcean((Biome::ID)v12)
							) {
							out[x + z * area_width] = v11;
						}
						else {
							out[x + z * area_width] = Biome::ID::BEACH;
						}
					}
					else {
						out[x + z * area_width] = Biome::ID::JUNGLE_EDGE;
					}
				}
				else if (
					v11 != Biome::ID::MOUNTAINS &&
					v11 != Biome::ID::WOODED_MOUNTAINS &&
					v11 != Biome::ID::MOUNTAIN_EDGE
					) {
					if (Biome::isSnowy((Biome::ID)biome)) {
						replaceOcean(out, x + z * area_width, v10, v21, v01, v12, v11, Biome::ID::SNOWY_BEACH);
					}
					else if (v11 != Biome::ID::BADLANDS && v11 != Biome::ID::WOODED_BADLANDS_PLATEAU) {
						if (
							v11 != Biome::ID::OCEAN &&
							v11 != Biome::ID::DEEP_OCEAN &&
							v11 != Biome::ID::RIVER &&
							v11 != Biome::ID::SWAMP
							) {
							if (
								!Biome::isOcean((Biome::ID)v10) &&
								!Biome::isOcean((Biome::ID)v21) &&
								!Biome::isOcean((Biome::ID)v01) &&
								!Biome::isOcean((Biome::ID)v12)
								) {
								out[x + z * area_width] = v11;
							}
							else {
								out[x + z * area_width] = Biome::ID::BEACH;
							}
						}
						else {
							out[x + z * area_width] = v11;
						}
					}
					else {
						if (
							!Biome::isOcean((Biome::ID)v10) &&
							!Biome::isOcean((Biome::ID)v21) &&
							!Biome::isOcean((Biome::ID)v01) &&
							!Biome::isOcean((Biome::ID)v12)
							) {
							if (
								Biome::getTypeCat((Biome::ID)v10) == Biome::TypeCat::BADLANDS &&
								Biome::getTypeCat((Biome::ID)v21) == Biome::TypeCat::BADLANDS &&
								Biome::getTypeCat((Biome::ID)v01) == Biome::TypeCat::BADLANDS &&
								Biome::getTypeCat((Biome::ID)v12) == Biome::TypeCat::BADLANDS
								) {
								out[x + z * area_width] = v11;
							}
							else {
								out[x + z * area_width] = Biome::ID::DESERT;
							}
						}
						else {
							out[x + z * area_width] = v11;
						}
					}
				}
				else {
					replaceOcean(out, x + z * area_width, v10, v21, v01, v12, v11, Biome::ID::STONE_SHORE);
				}
			}
		}
	}

	void riverMix(Layer* layer, int32_t* __restrict out, int32_t area_x, int32_t area_z, int32_t area_width, int32_t area_height) {
		int32_t idx;
		int32_t len;
		int32_t* buf;

		if (layer->parent_2 == NULL) {
			printf("riverMix() requires two parents! Use setupMultiLayer()\n");
			exit(1);
		}

		len = area_width * area_height;
		buf = (int*)malloc(len * sizeof(int));

		layer->parent->getMap(layer->parent, out, area_x, area_z, area_width, area_height); // biome chain
		memcpy(buf, out, len * sizeof(int));

		layer->parent_2->getMap(layer->parent_2, out, area_x, area_z, area_width, area_height); // rivers

		for (idx = 0; idx < len; idx++) {
			if (Biome::isOcean((Biome::ID)buf[idx])) {
				out[idx] = buf[idx];
			}
			else {
				if (out[idx] == Biome::ID::RIVER) {
					if (buf[idx] == Biome::ID::SNOWY_TUNDRA) {
						out[idx] = Biome::ID::FROZEN_RIVER;
					}
					else if (buf[idx] == Biome::ID::MUSHROOM_FIELDS || buf[idx] == Biome::ID::MUSHROOM_FIELD_SHORE) {
						out[idx] = Biome::ID::MUSHROOM_FIELD_SHORE;
					}
					else {
						out[idx] = out[idx] & 255;
					}
				}
				else {
					out[idx] = buf[idx];
				}
			}
		}

		free(buf);
	}

	void oceanTemp(Layer* layer, int32_t* __restrict out, int32_t area_x, int32_t area_z, int32_t area_width, int32_t area_height) {
		OceanGen* gen = layer->ocean_gen;

		for (int32_t z = 0; z < area_height; z++) {
			for (int32_t x = 0; x < area_width; x++) {
				double tmp = gen->getTemp((x + area_x) / 8.0, (z + area_z) / 8.0, 0);

				if (tmp > 0.4) {
					out[x + z * area_width] = Biome::ID::WARM_OCEAN;
				}
				else if (tmp > 0.2) {
					out[x + z * area_width] = Biome::ID::LUKEWARM_OCEAN;
				}
				else if (tmp < -0.4) {
					out[x + z * area_width] = Biome::ID::FROZEN_OCEAN;
				}
				else if (tmp < -0.2) {
					out[x + z * area_width] = Biome::ID::COLD_OCEAN;
				}
				else {
					out[x + z * area_width] = Biome::ID::OCEAN;
				}
			}
		}
	}

	void oceanMix(Layer* layer, int32_t* __restrict out, int32_t area_x, int32_t area_z, int32_t area_width, int32_t area_height) {
		int32_t land_x = area_x - 8, land_z = area_z - 8;
		int32_t land_width = area_width + 17, land_height = area_height + 17;
		int32_t* map_1;
		int32_t* map_2;

		if (layer->parent_2 == NULL) {
			printf("oceanMix() requires two parents! Use setupMultiLayer()\n");
			exit(1);
		}

		layer->parent->getMap(layer->parent, out, land_x, land_z, land_width, land_height);
		map_1 = (int32_t*)malloc(land_width * land_height * sizeof(int));
		memcpy(map_1, out, land_width * land_height * sizeof(int));

		layer->parent_2->getMap(layer->parent_2, out, area_x, area_z, area_width, area_height);
		map_2 = (int32_t*)malloc(area_width * area_height * sizeof(int));
		memcpy(map_2, out, area_width * area_height * sizeof(int));

		for (int32_t z = 0; z < area_height; z++) {
			for (int32_t x = 0; x < area_width; x++) {
				int32_t land_id = map_1[(x + 8) + (z + 8) * land_width];
				int32_t ocean_id = map_2[x + z * area_width];

				if (!Biome::isOcean((Biome::ID)land_id)) {
					out[x + z * area_width] = land_id;
					continue;
				}

				for (int32_t i = -8; i <= 8; i += 4) {
					for (int32_t j = -8; j <= 8; j += 4) {
						int32_t nearby_id = map_1[(x + i + 8) + (z + j + 8) * land_width];

						if (Biome::isOcean((Biome::ID)nearby_id)) {
							continue;
						}

						if (ocean_id == Biome::ID::WARM_OCEAN) {
							out[x + z * area_width] = Biome::ID::LUKEWARM_OCEAN;
							goto loop_x;
						}

						if (ocean_id == Biome::ID::FROZEN_OCEAN) {
							out[x + z * area_width] = Biome::ID::COLD_OCEAN;
							goto loop_x;
						}
					}
				}

				if (land_id == Biome::ID::DEEP_OCEAN) {
					switch ((Biome::ID)ocean_id) {
					case Biome::ID::LUKEWARM_OCEAN:
						ocean_id = Biome::ID::DEEP_LUKEWARM_OCEAN;
						break;
					case Biome::ID::OCEAN:
						ocean_id = Biome::ID::DEEP_OCEAN;
						break;
					case Biome::ID::COLD_OCEAN:
						ocean_id = Biome::ID::DEEP_COLD_OCEAN;
						break;
					case Biome::ID::FROZEN_OCEAN:
						ocean_id = Biome::ID::DEEP_FROZEN_OCEAN;
						break;
					default:
						break;
					}
				}

				out[x + z * area_width] = ocean_id;

			loop_x:;
			}
		}

		free(map_1);
		free(map_2);
	}

	void voronoiZoom(Layer* layer, int32_t* __restrict out, int32_t area_x, int32_t area_z, int32_t area_width, int32_t area_height) {
		area_x -= 2;
		area_z -= 2;

		int32_t parent_x = area_x >> 2;
		int32_t parent_z = area_z >> 2;
		int32_t parent_width = (area_width >> 2) + 2;
		int32_t parent_height = (area_height >> 2) + 2;
		int32_t new_width = (parent_width - 1) << 2;
		int32_t new_height = (parent_height - 1) << 2;
		int32_t* buf = (int32_t*)malloc((new_width + 1) * (new_height + 1) * sizeof(*buf));

		layer->parent->getMap(layer->parent, out, parent_x, parent_z, parent_width, parent_height);

		for (int32_t z = 0; z < parent_height - 1; z++) {
			int32_t v00 = out[(z + 0) * parent_width];
			int32_t v01 = out[(z + 1) * parent_width];

			for (int32_t x = 0; x < parent_width - 1; x++) {
				layer->setChunkSeed((x + parent_x) << 2, (z + parent_z) << 2);
				double da1 = (layer->mcNextInt(1024) / 1024.0 - 0.5) * 3.6;
				double da2 = (layer->mcNextInt(1024) / 1024.0 - 0.5) * 3.6;

				layer->setChunkSeed((x + parent_x + 1) << 2, (z + parent_z) << 2);
				double db1 = (layer->mcNextInt(1024) / 1024.0 - 0.5) * 3.6 + 4.0;
				double db2 = (layer->mcNextInt(1024) / 1024.0 - 0.5) * 3.6;

				layer->setChunkSeed((x + parent_x) << 2, (z + parent_z + 1) << 2);
				double dc1 = (layer->mcNextInt(1024) / 1024.0 - 0.5) * 3.6;
				double dc2 = (layer->mcNextInt(1024) / 1024.0 - 0.5) * 3.6 + 4.0;

				layer->setChunkSeed((x + parent_x + 1) << 2, (z + parent_z + 1) << 2);
				double dd1 = (layer->mcNextInt(1024) / 1024.0 - 0.5) * 3.6 + 4.0;
				double dd2 = (layer->mcNextInt(1024) / 1024.0 - 0.5) * 3.6 + 4.0;

				int32_t v10 = out[x + 1 + (z + 0) * parent_width] & 255;
				int32_t v11 = out[x + 1 + (z + 1) * parent_width] & 255;

				for (int32_t j = 0; j < 4; j++) {
					int32_t idx = ((z << 2) + j) * new_width + (x << 2);

					for (int32_t i = 0; i < 4; i++) {
						double da = (j - da2) * (j - da2) + (i - da1) * (i - da1);
						double db = (j - db2) * (j - db2) + (i - db1) * (i - db1);
						double dc = (j - dc2) * (j - dc2) + (i - dc1) * (i - dc1);
						double dd = (j - dd2) * (j - dd2) + (i - dd1) * (i - dd1);

						if (da < db && da < dc && da < dd) {
							buf[idx++] = v00;
						}
						else if (db < da && db < dc && db < dd) {
							buf[idx++] = v10;
						}
						else if (dc < da && dc < db && dc < dd) {
							buf[idx++] = v01;
						}
						else {
							buf[idx++] = v11;
						}
					}
				}

				v00 = v10;
				v01 = v11;
			}
		}

		for (int32_t z = 0; z < area_height; z++) {
			memcpy(&out[z * area_width], &buf[(z + (area_z & 3)) * new_width + (area_x & 3)], area_width * sizeof(int32_t));
		}

		free(buf);
	}
}