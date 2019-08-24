#ifndef NBT_TAGDOUBLE_HPP
#define NBT_TAGDOUBLE_HPP

#include <cstdint>

#include "Tag.hpp"

namespace NBT {

	class TagDouble : public Tag {
		double value = 0.0;

	public:
		TagDouble();

		void load(Deserializer* stream);

		void save(Serializer* stream);

		std::string asString();

		double getValue();

		void setValue(double value_);

	};

}

#endif