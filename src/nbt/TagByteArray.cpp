#include "TagByteArray.hpp"

namespace NBT {

	TagByteArray::TagByteArray() {
		type = Tag::Type::BYTE_ARRAY;
	}

	void TagByteArray::load(Deserializer* stream) {
		int32_t arr_len = ((int32_t)stream->get() << 24) +
						  ((int32_t)stream->get() << 16) +
						  ((int32_t)stream->get() <<  8) +
						  ((int32_t)stream->get());
		if (arr_len < 0) {
			stream->error("Array length is less than 0.");
		}
		for (int32_t i = 0; i < arr_len; i++) {
			values.push_back((int8_t)stream->get());
		}
	}

	void TagByteArray::save(Serializer* stream) {
		size_t arr_len_v = values.size();
		if (arr_len_v > 2147483647) {
			stream->error("Array length is greater than 2147483647.");
		}
		int32_t arr_len = (int32_t)values.size();
		stream->put(std::byte(arr_len >> 24 & 0xFF));
		stream->put(std::byte(arr_len >> 16 & 0xFF));
		stream->put(std::byte(arr_len >>  8 & 0xFF));
		stream->put(std::byte(arr_len       & 0xFF));
		for (int32_t i = 0; i < arr_len; i++) {
			stream->put(std::byte(values[i]));
		}
	}

	std::string TagByteArray::asString() {
		std::string out = "[B;";
		for (size_t i = 0; i < values.size(); i++) {
			if (i != 0) {
				out += ",";
			}
			out.append(std::to_string(values[i]) + "b");
		}
		out += "]";
		return out;
	}

	TagByteArray::Iterator TagByteArray::begin() {
		return values.begin();
	}

	TagByteArray::Iterator TagByteArray::end() {
		return values.end();
	}

	size_t TagByteArray::items() {
		return values.size();
	}

	size_t TagByteArray::size() {
		return values.size();
	}

	std::vector<int8_t>& TagByteArray::data() {
		return values;
	}

}