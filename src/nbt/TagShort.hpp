#ifndef NBT_TAGSHORT_HPP
#define NBT_TAGSHORT_HPP

#include <cstdint>

#include "Tag.hpp"

namespace NBT {

	class TagShort : public Tag {
		int16_t value = 0;

	public:
		TagShort();

		void load(Deserializer* stream);

		void save(Serializer* stream);

		std::string asString();

		int16_t getValue();

		void setValue(int16_t value_);

	};

}

#endif