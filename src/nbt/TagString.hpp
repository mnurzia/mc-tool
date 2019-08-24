#ifndef NBT_TAGSTRING_HPP
#define NBT_TAGSTRING_HPP

#include <cstdint>

#include "Tag.hpp"

namespace NBT {

	class TagString : public Tag {
		std::string value;

	public:
		TagString();

		void load(Deserializer* stream);

		void save(Serializer* stream);

		std::string asString();

		std::string& getValue();

		void setValue(std::string& value_);

	};

}

#endif