#ifndef NBT_TAGINTARRAY_HPP
#define NBT_TAGINTARRAY_HPP

#include <cstdint>

#include "Tag.hpp"

namespace NBT {

	class TagIntArray : public Tag {
		std::vector<int32_t> values;
		typedef std::vector<int32_t>::iterator Iterator;

	public:
		TagIntArray();

		void load(Deserializer* stream);

		void save(Serializer* stream);

		std::string asString();

		Iterator begin();

		Iterator end();

		size_t items();

		size_t size();

		std::vector<int32_t>& data();

	};

}

#endif