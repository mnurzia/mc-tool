#include "TagLong.hpp"

namespace NBT {

	TagLong::TagLong() {
		type = Tag::Type::LONG;
	}

	void TagLong::load(Deserializer* stream) {
		value = ((int64_t)stream->get() << 56) +
				((int64_t)stream->get() << 48) +
				((int64_t)stream->get() << 40) +
				((int64_t)stream->get() << 32) +
				((int64_t)stream->get() << 24) +
				((int64_t)stream->get() << 16) + 
				((int64_t)stream->get() <<  8) +
				((int64_t)stream->get());
	}

	void TagLong::save(Serializer* stream) {
		stream->put(std::byte(value >> 56 & 0xFF));
		stream->put(std::byte(value >> 48 & 0xFF));
		stream->put(std::byte(value >> 40 & 0xFF));
		stream->put(std::byte(value >> 32 & 0xFF));
		stream->put(std::byte(value >> 24 & 0xFF));
		stream->put(std::byte(value >> 16 & 0xFF));
		stream->put(std::byte(value >>  8 & 0xFF));
		stream->put(std::byte(value       & 0xFF));
	}

	std::string TagLong::asString() {
		return std::to_string(value) + "l";
	}

	int64_t TagLong::getValue() {
		return value;
	}

	void TagLong::setValue(int64_t value_) {
		value = value_;
	}

}