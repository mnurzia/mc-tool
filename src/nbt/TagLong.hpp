#ifndef NBT_TAGLONG_HPP
#define NBT_TAGLONG_HPP

#include <cstdint>

#include "Tag.hpp"

namespace NBT {

	class TagLong : public Tag {
		int64_t value;

	public:
		TagLong();

		void load(Deserializer* stream);

		void save(Serializer* stream);

		std::string asString();

		int64_t getValue();

		void setValue(int64_t value_);

	};

}

#endif