#include "Feature.hpp"

namespace WorldGen {

	const std::string feature_names[] = {
		"Mineshaft",
		"Buried Treasure",
		"Desert Pyramid",
		"End City",
		"Igloo",
		"Jungle Temple",
		"Nether Fortress",
		"Ocean Monument",
		"Ocean Ruin",
		"Pillager Outpost",
		"Shipwreck",
		"Stronghold",
		"Witch Hut",
		"Village",
		"Woodland Mansion"
	};

	Feature::Feature(int32_t x_, int32_t z_, Feature::Type type_) {
		x = x_;
		z = z_;
		variant = type_;
	}

	int32_t Feature::getX() {
		return x;
	}

	int32_t Feature::getZ() {
		return z;
	}

	Feature::Type Feature::getType() {
		return variant;
	}

	std::string Feature::getName() {
		return feature_names[int32_t(variant)];
	}

	bool Feature::operator==(Feature& other) {
		return (other.getX() == getX()) && (other.getZ() == getZ()) && (other.getType() == getType());
	}

}