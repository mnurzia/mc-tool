#ifndef NBT_TAGINT_HPP
#define NBT_TAGINT_HPP

#include <cstdint>

#include "Tag.hpp"

namespace NBT {

	class TagInt : public Tag {
		int32_t value = 0;

	public:
		TagInt();

		void load(Deserializer* stream);

		void save(Serializer* stream);

		std::string asString();

		int32_t getValue();

		void setValue(int32_t value_);

	};

}

#endif