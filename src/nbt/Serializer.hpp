#ifndef NBT_SERIALIZER_HPP
#define NBT_SERIALIZER_HPP

#include <cstddef>

#include <string>

#include "SerialBase.hpp"

namespace NBT {

	class Serializer : public SerialBase {

	public:
		void put(std::byte in);

	};

}

#endif