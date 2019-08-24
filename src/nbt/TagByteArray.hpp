#ifndef NBT_TAGBYTEARRAY_HPP
#define NBT_TAGBYTEARRAY_HPP

#include <cstdint>

#include "Tag.hpp"

namespace NBT {

	class TagByteArray : public Tag {
		std::vector<int8_t> values;
		typedef std::vector<int8_t>::iterator Iterator;

	public:
		TagByteArray();

		void load(Deserializer* stream);

		void save(Serializer* stream);

		std::string asString();

		Iterator begin();

		Iterator end();

		size_t items();

		size_t size();

		std::vector<int8_t>& data();

	};

}

#endif