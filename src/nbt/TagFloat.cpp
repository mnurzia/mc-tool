#include "TagFloat.hpp"

namespace NBT {

	TagFloat::TagFloat() {
		type = Tag::Type::FLOAT;
	}

	void TagFloat::load(Deserializer* stream) {
		uint8_t* ptr = ((uint8_t*)(&value)) + 4;
		for (int i = 0; i < 4; i++) {
			ptr--;
			*ptr = (uint8_t)stream->get();
		}
	}

	void TagFloat::save(Serializer* stream) {
		uint8_t* ptr = ((uint8_t*)(&value)) + 4;
		for (int i = 0; i < 4; i++) {
			ptr--;
			stream->put((std::byte)*ptr);
		}
	}

	std::string TagFloat::asString() {
		return std::to_string(value) + "f";
	}

	float TagFloat::getValue() {
		return value;
	}

	void TagFloat::setValue(float value_) {
		value = value;
	}

}