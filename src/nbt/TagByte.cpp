#include "TagByte.hpp"

namespace NBT {

	TagByte::TagByte() {
		type = Tag::Type::BYTE;
	}

	void TagByte::load(Deserializer* stream) {
		value = (int8_t)stream->get();
	}

	void TagByte::save(Serializer* stream) {
		stream->put(std::byte(value));
	}

	std::string TagByte::asString() {
		return std::to_string(value) + "b";
	}

	int8_t TagByte::getValue() {
		return value;
	}

	void TagByte::setValue(int8_t value_) {
		value = value_;
	}

}