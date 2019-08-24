#ifndef NBT_TAGFLOAT_HPP
#define NBT_TAGFLOAT_HPP

#include <cstdint>

#include "Tag.hpp"

namespace NBT {

	class TagFloat : public Tag {
		float value = 0.0f;

	public:
		TagFloat();

		void load(Deserializer* stream);

		void save(Serializer* stream);

		std::string asString();

		float getValue();

		void setValue(float value_);

	};

}

#endif