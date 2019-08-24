#include "TagLongArray.hpp"

namespace NBT {

	TagLongArray::TagLongArray() {
		type = Tag::Type::LONG_ARRAY;
	}

	void TagLongArray::load(Deserializer* stream) {
		int32_t arr_len = ((int32_t)stream->get() << 24) +
						  ((int32_t)stream->get() << 16) +
						  ((int32_t)stream->get() <<  8) +
						  ((int32_t)stream->get());
		if (arr_len < 0) {
			stream->error("Array length is less than 0.");
		}
		for (int32_t i = 0; i < arr_len; i++) {
			values.push_back(
				((int64_t)stream->get() << 56) +
				((int64_t)stream->get() << 48) +
				((int64_t)stream->get() << 40) +
				((int64_t)stream->get() << 32) +
				((int64_t)stream->get() << 24) +
				((int64_t)stream->get() << 16) +
				((int64_t)stream->get() << 8) +
				((int64_t)stream->get())
			);
		}
	}

	void TagLongArray::save(Serializer* stream) {
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
			stream->put(std::byte(values[i] >> 56 & 0xFF));
			stream->put(std::byte(values[i] >> 48 & 0xFF));
			stream->put(std::byte(values[i] >> 40 & 0xFF));
			stream->put(std::byte(values[i] >> 32 & 0xFF));
			stream->put(std::byte(values[i] >> 24 & 0xFF));
			stream->put(std::byte(values[i] >> 16 & 0xFF));
			stream->put(std::byte(values[i] >>  8 & 0xFF));
			stream->put(std::byte(values[i]       & 0xFF));
		}
	}

	std::string TagLongArray::asString() {
		std::string out = "[L;";
		for (size_t i = 0; i < values.size(); i++) {
			if (i != 0) {
				out += ",";
			}
			out.append(std::to_string(values[i]) + "l");
		}
		out += "]";
		return out;
	}

	TagLongArray::Iterator TagLongArray::begin() {
		return values.begin();
	}

	TagLongArray::Iterator TagLongArray::end() {
		return values.end();
	}

	size_t TagLongArray::items() {
		return values.size();
	}

	size_t TagLongArray::size() {
		return values.size();
	}

	std::vector<int64_t>& TagLongArray::data() {
		return values;
	}

}