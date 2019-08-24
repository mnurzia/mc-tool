#ifndef NBT_TAG_HPP
#define NBT_TAG_HPP

#include <string>

#include "Deserializer.hpp"
#include "Serializer.hpp"

namespace NBT {

	extern const char* TagNames[];

	class Tag {

	public:
		enum class Type : int8_t {
			END = 0,
			BYTE = 1,
			SHORT = 2,
			INT = 3,
			LONG = 4,
			FLOAT = 5,
			DOUBLE = 6,
			BYTE_ARRAY = 7,
			STRING = 8,
			LIST = 9,
			COMPOUND = 10,
			INT_ARRAY = 11,
			LONG_ARRAY = 12,
			COUNT
		};

	protected:
		Type type;

	public:
		Type getType();

		virtual void load(Deserializer* stream);

		virtual void save(Serializer* stream);

		virtual std::string asString();

		virtual ~Tag() = default;

	};

}

#endif