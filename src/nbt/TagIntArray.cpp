#include "TagIntArray.hpp"

namespace NBT {

	TagIntArray::TagIntArray() {
		type = Tag::Type::INT_ARRAY;
	}

	void TagIntArray::load(Deserializer* stream) {
		int32_t arr_len = ((int32_t)stream->get() << 24) +
						  ((int32_t)stream->get() << 16) +
						  ((int32_t)stream->get() <<  8) +
						  ((int32_t)stream->get());
		if (arr_len < 0) {
			stream->error("Array length is less than 0.");
		}
		for (int32_t i = 0; i < arr_len; i++) {
			values.push_back(
				((int32_t)stream->get() << 24) +
				((int32_t)stream->get() << 16) +
				((int32_t)stream->get() << 8) +
				((int32_t)stream->get())
			);
		}
	}

	void TagIntArray::save(Serializer* stream) {
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
			stream->put(std::byte(values[i] >> 24 & 0xFF));
			stream->put(std::byte(values[i] >> 16 & 0xFF));
			stream->put(std::byte(values[i] >>  8 & 0xFF));
			stream->put(std::byte(values[i]       & 0xFF));
		}
	}

	std::string TagIntArray::asString() {
		std::string out = "[I;";
		for (size_t i = 0; i < values.size(); i++) {
			if (i != 0) {
				out += ",";
			}
			out.append(std::to_string(values[i]));
		}
		out += "]";
		return out;
	}

	TagIntArray::Iterator TagIntArray::begin() {
		return values.begin();
	}

	TagIntArray::Iterator TagIntArray::end() {
		return values.end();
	}

	size_t TagIntArray::items() {
		return values.size();
	}

	size_t TagIntArray::size() {
		return values.size();
	}

	std::vector<int32_t>& TagIntArray::data() {
		return values;
	}

}