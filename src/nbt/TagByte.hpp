#ifndef NBT_TAGBYTE_HPP
#define NBT_TAGBYTE_HPP

#include <cstdint>

#include "Tag.hpp"

namespace NBT {

	class TagByte : public Tag {
		int8_t value = 0;

	public:
		TagByte();

		void load(Deserializer* stream);

		void save(Serializer* stream);

		std::string asString();

		int8_t getValue();

		void setValue(int8_t value_);

	};

}

#endif