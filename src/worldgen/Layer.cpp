#include "Layer.hpp"

#include "Map.hpp"

namespace WorldGen {

	void Layer::getMaxArea(int32_t area_x, int32_t area_z, int32_t* max_x, int32_t* max_z) {
		if (getMap == Map::zoom) {
			area_x = (area_x >> 1) + 2;
			area_z = (area_z >> 1) + 2;
		}
		else if (getMap == Map::voronoiZoom) {
			area_x = (area_x >> 2) + 2;
			area_z = (area_z >> 2) + 2;
		}
		else if (getMap == Map::oceanMix) {
			area_x += 17;
			area_z += 17;
		}
		else {
			if (
				getMap != Map::null &&
				getMap != Map::skip &&
				getMap != Map::island &&
				getMap != Map::special &&
				getMap != Map::biome &&
				getMap != Map::riverInit &&
				getMap != Map::riverMix &&
				getMap != Map::oceanTemp
				) {
				area_x += 2;
				area_z += 2;
			}
		}

		if (area_x > * max_x) {
			*max_x = area_x;
		}
		if (area_z > * max_z) {
			*max_z = area_z;
		}

		if (parent != NULL) {
			parent->getMaxArea(area_x, area_z, max_x, max_z);
		}
		if (parent_2 != NULL) {
			parent_2->getMaxArea(area_x, area_z, max_x, max_z);
		}
	}

	int32_t Layer::calcRequiredBuf(int32_t area_x, int32_t area_z) {
		int32_t max_x = area_x;
		int32_t max_z = area_z;
		getMaxArea(area_x, area_z, &max_x, &max_z);

		return max_x * max_z;
	}

	int32_t* Layer::allocCache(int32_t size_x, int32_t size_z) {
		int32_t sz = calcRequiredBuf(size_x, size_z);

		int32_t* ret = (int32_t*)malloc(sizeof(int32_t) * sz);
		if (ret == NULL) {
			printf("Couldn't allocate cache!\n");
			exit(1);
		}
		memset(ret, 0, sizeof(*ret) * sz);

		return ret;
	}

	void Layer::setWorldSeed(int64_t seed) {
		if (parent_2 != NULL && getMap != Map::hills) {
			parent_2->setWorldSeed(seed);
		}

		if (parent != NULL) {
			parent->setWorldSeed(seed);
		}

		if (ocean_gen != NULL) {
			ocean_gen->init(seed);
		}

		world_seed = seed;
		world_seed *= world_seed * 6364136223846793005LL + 1442695040888963407LL;
		world_seed += base_seed;
		world_seed *= world_seed * 6364136223846793005LL + 1442695040888963407LL;
		world_seed += base_seed;
		world_seed *= world_seed * 6364136223846793005LL + 1442695040888963407LL;
		world_seed += base_seed;
	}

	void Layer::genArea(int32_t* out, int32_t area_x, int32_t area_z, int32_t area_width, int32_t area_height) {
		memset(out, 0, area_width * area_height * sizeof(*out));
		getMap(this, out, area_x, area_z, area_width, area_height);
	}

}