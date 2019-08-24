#include "TagString.hpp"

namespace NBT {

	TagString::TagString() {
		type = Tag::Type::STRING;
	}

	void TagString::load(Deserializer* stream) {
		uint16_t str_len = (((uint16_t)stream->get()) << 8) + (uint16_t)stream->get();
		for (uint16_t i = 0; i < str_len; i++) {
			value.push_back((char)stream->get());
		}
	}

	void TagString::save(Serializer* stream) {
		if (value.length() > 65535) {
			stream->error("Value length greater than 65535.");
		}
		for (size_t i = 0; i < value.length(); i++) {
			stream->put((std::byte)value[i]);
		}
	}

	std::string TagString::asString() {
		std::string out = "\"";
		for (size_t i = 0; i < value.length(); i++) {
			char cur_val = value[i];
			if (cur_val == '"') {
				out.append("\\\"");
			} else {
				out.push_back(cur_val);
			}
		}
		out += "\"";
		return out;
	}

	std::string& TagString::getValue() {
		return value;
	}

	void TagString::setValue(std::string& value_) {
		value = value_;
	}

}