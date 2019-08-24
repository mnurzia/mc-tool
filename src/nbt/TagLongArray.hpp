#ifndef NBT_TAGLONGARRAY_HPP
#define NBT_TAGLONGARRAY_HPP

#include <cstdint>

#include "Tag.hpp"

namespace NBT {

	class TagLongArray : public Tag {
		std::vector<int64_t> values;
		typedef std::vector<int64_t>::iterator Iterator;

	public:
		TagLongArray();

		void load(Deserializer* stream);

		void save(Serializer* stream);

		std::string asString();

		Iterator begin();

		Iterator end();

		size_t items();

		size_t size();

		std::vector<int64_t>& data();

	};

}

#endif