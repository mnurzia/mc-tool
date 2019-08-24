#include "TagDouble.hpp"

namespace NBT {

	TagDouble::TagDouble() {
		type = Tag::Type::DOUBLE;
	}

	void TagDouble::load(Deserializer* stream) {
		uint8_t* ptr = ((uint8_t*)(&value)) + 8;
		for (int i = 0; i < 8; i++) {
			ptr--;
			*ptr = (uint8_t)stream->get();
		}
	}

	void TagDouble::save(Serializer* stream) {
		uint8_t* ptr = ((uint8_t*)(&value)) + 8;
		for (int i = 0; i < 8; i++) {
			ptr--;
			stream->put((std::byte)*ptr);
		}
	}

	std::string TagDouble::asString() {
		return std::to_string(value) + "d";
	}

	double TagDouble::getValue() {
		return value;
	}

	void TagDouble::setValue(double value_) {
		value = value_;
	}

}