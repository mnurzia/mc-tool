#ifndef UTIL_UUID_HPP
#define UTIL_UUID_HPP

#include <cstdint>

#include <string>

namespace Util {
	
	class UUID {
	public:
		uint64_t contents[2] = { 0, 0 };

		UUID();
		UUID(uint64_t c0, uint64_t c1);
		UUID(uint32_t a, uint16_t b, uint16_t c, uint16_t d, uint64_t e);
		UUID(std::string other);

		bool operator==(UUID const& b) const;

		std::string as_string() const;
	};

	class UUIDHash {

	public:

		size_t operator()(const UUID& p) const;
	};

}

#endif