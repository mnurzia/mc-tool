#include "TagShort.hpp"

namespace NBT {

	TagShort::TagShort() {
		type = Tag::Type::SHORT;
	}

	void TagShort::load(Deserializer* stream) {
		value = ((int16_t)stream->get() << 8 ) + (int16_t)stream->get();
	}

	void TagShort::save(Serializer* stream) {
		stream->put(std::byte(value >> 8));
		stream->put(std::byte(value & 0xFF));
	}

	std::string TagShort::asString() {
		return std::to_string(value) + "s";
	}

	int16_t TagShort::getValue() {
		return value;
	}

	void TagShort::setValue(int16_t value_) {
		value = value_;
	}

}