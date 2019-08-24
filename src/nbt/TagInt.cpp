#include "TagInt.hpp"

namespace NBT {

	TagInt::TagInt() {
		type = Tag::Type::INT;
	}

	void TagInt::load(Deserializer* stream) {
		value = ((int32_t)stream->get() << 24) +
				((int32_t)stream->get() << 16) + 
				((int32_t)stream->get() <<  8) +
				((int32_t)stream->get());
	}

	void TagInt::save(Serializer* stream) {
		stream->put(std::byte(value >> 24 & 0xFF));
		stream->put(std::byte(value >> 16 & 0xFF));
		stream->put(std::byte(value >>  8 & 0xFF));
		stream->put(std::byte(value       & 0xFF));
	}

	std::string TagInt::asString() {
		return std::to_string(value);
	}

	int32_t TagInt::getValue() {
		return value;
	}

	void TagInt::setValue(int32_t value_) {
		value = value_;
	}

}