#include "UUID.hpp"

#include <random>

namespace Util {

	static uint64_t rand_x = 0x2CC5E1B03E54CAAC, rand_y = 0xA1BA32C97E46BCDC, rand_z = 0x20764BF4D6107231;

	static uint64_t fast_rand() {
		rand_x ^= rand_x << 16;
		rand_x ^= rand_x >> 5;
		rand_x ^= rand_x << 1;

		uint64_t t = rand_x;
		rand_x = rand_y;
		rand_y = rand_z;
		rand_z = t ^ rand_x ^ rand_y;

		return rand_z;
	}

	UUID::UUID() {
		contents[0] = fast_rand();
		contents[1] = fast_rand();
		contents[0] = contents[0] & ~(uint64_t(0xF) << 48);
		contents[0] = contents[0] |  (uint64_t(0x4) << 48);
		contents[1] = contents[1] & ~(uint64_t(0xC) <<  0);
		contents[1] = contents[1] |  (uint64_t(0x8) <<  0);
	}

	UUID::UUID(uint64_t c0, uint64_t c1) {
		contents[0] = c0;
		contents[1] = c1;
	}

	UUID::UUID(uint32_t a, uint16_t b, uint16_t c, uint16_t d, uint64_t e) {
		contents[0] = a;
		contents[0] |= uint64_t(b) << 32;
		contents[0] |= uint64_t(c) << 48;
		contents[1] = d;
		contents[1] |= uint64_t(e) << 16;
	}

	static inline bool chr_hex(char in, char* out) {
		if (in >= '0' && in <= '9') {
			*out = in - '0';
			return true;
		} else if (in >= 'A' && in <= 'F') {
			*out = in - 'A';
			return true;
		} else if (in >= 'a' && in <= 'f') {
			*out = in - 'a';
			return true;
		}
		return false;
	}

	UUID::UUID(std::string other) {
		bool valid = true;
		if (other.length() != 36) {
			UUID();
			return;
		}
		char out = 0;
		if (chr_hex(other[ 0], &out)) { contents[0] |= uint64_t(out) <<  0; } else { goto error; }
		if (chr_hex(other[ 1], &out)) { contents[0] |= uint64_t(out) <<  4; } else { goto error; }
		if (chr_hex(other[ 2], &out)) { contents[0] |= uint64_t(out) <<  8; } else { goto error; }
		if (chr_hex(other[ 3], &out)) { contents[0] |= uint64_t(out) << 12; } else { goto error; }
		if (chr_hex(other[ 4], &out)) { contents[0] |= uint64_t(out) << 16; } else { goto error; }
		if (chr_hex(other[ 5], &out)) { contents[0] |= uint64_t(out) << 20; } else { goto error; }
		if (chr_hex(other[ 6], &out)) { contents[0] |= uint64_t(out) << 24; } else { goto error; }
		if (chr_hex(other[ 7], &out)) { contents[0] |= uint64_t(out) << 28; } else { goto error; }
		if (other[ 8] != '-') { goto error; }
		if (chr_hex(other[ 9], &out)) { contents[0] |= uint64_t(out) << 32; } else { goto error; }
		if (chr_hex(other[10], &out)) { contents[0] |= uint64_t(out) << 36; } else { goto error; }
		if (chr_hex(other[11], &out)) { contents[0] |= uint64_t(out) << 40; } else { goto error; }
		if (chr_hex(other[12], &out)) { contents[0] |= uint64_t(out) << 44; } else { goto error; }
		if (other[13] != '-') { goto error; }
		if (chr_hex(other[14], &out)) { contents[0] |= uint64_t(out) << 48; } else { goto error; }
		if (chr_hex(other[15], &out)) { contents[0] |= uint64_t(out) << 52; } else { goto error; }
		if (chr_hex(other[16], &out)) { contents[0] |= uint64_t(out) << 56; } else { goto error; }
		if (chr_hex(other[17], &out)) { contents[0] |= uint64_t(out) << 60; } else { goto error; }
		if (other[18] != '-') { goto error; }
		if (chr_hex(other[19], &out)) { contents[1] |= uint64_t(out) <<  0; } else { goto error; }
		if (chr_hex(other[20], &out)) { contents[1] |= uint64_t(out) <<  4; } else { goto error; }
		if (chr_hex(other[21], &out)) { contents[1] |= uint64_t(out) <<  8; } else { goto error; }
		if (chr_hex(other[22], &out)) { contents[1] |= uint64_t(out) << 12; } else { goto error; }
		if (other[23] != '-') { goto error; }
		if (chr_hex(other[24], &out)) { contents[1] |= uint64_t(out) << 16; } else { goto error; }
		if (chr_hex(other[25], &out)) { contents[1] |= uint64_t(out) << 20; } else { goto error; }
		if (chr_hex(other[26], &out)) { contents[1] |= uint64_t(out) << 24; } else { goto error; }
		if (chr_hex(other[27], &out)) { contents[1] |= uint64_t(out) << 28; } else { goto error; }
		if (chr_hex(other[28], &out)) { contents[1] |= uint64_t(out) << 32; } else { goto error; }
		if (chr_hex(other[29], &out)) { contents[1] |= uint64_t(out) << 36; } else { goto error; }
		if (chr_hex(other[30], &out)) { contents[1] |= uint64_t(out) << 40; } else { goto error; }
		if (chr_hex(other[31], &out)) { contents[1] |= uint64_t(out) << 44; } else { goto error; }
		if (chr_hex(other[32], &out)) { contents[1] |= uint64_t(out) << 48; } else { goto error; }
		if (chr_hex(other[33], &out)) { contents[1] |= uint64_t(out) << 52; } else { goto error; }
		if (chr_hex(other[34], &out)) { contents[1] |= uint64_t(out) << 56; } else { goto error; }
		if (chr_hex(other[35], &out)) { contents[1] |= uint64_t(out) << 60; } else { goto error; }
	error:
		UUID();
		return;
	}

	bool UUID::operator==(UUID const& b) const {
		return (b.contents[0] == contents[0]) && (b.contents[1] == contents[1]);
	}

	static inline uint8_t hex_chr(uint8_t in) {
		if (in < 10) {
			return '0' + in;
		} else {
			return 'A' + in - 10;
		}
	}

	std::string UUID::as_string() const {
		char out[37];
		out[0]  = hex_chr((contents[0] >>  0) & 0xF);
		out[1]  = hex_chr((contents[0] >>  4) & 0xF);
		out[2]  = hex_chr((contents[0] >>  8) & 0xF);
		out[3]  = hex_chr((contents[0] >> 12) & 0xF);
		out[4]  = hex_chr((contents[0] >> 16) & 0xF);
		out[5]  = hex_chr((contents[0] >> 20) & 0xF);
		out[6]  = hex_chr((contents[0] >> 24) & 0xF);
		out[7]  = hex_chr((contents[0] >> 28) & 0xF);

		out[8]  = '-';

		out[9]  = hex_chr((contents[0] >> 32) & 0xF);
		out[10] = hex_chr((contents[0] >> 36) & 0xF);
		out[11] = hex_chr((contents[0] >> 40) & 0xF);
		out[12] = hex_chr((contents[0] >> 44) & 0xF);

		out[13] = '-';
		
		out[14] = hex_chr((contents[0] >> 48) & 0xF);
		out[15] = hex_chr((contents[0] >> 52) & 0xF);
		out[16] = hex_chr((contents[0] >> 56) & 0xF);
		out[17] = hex_chr((contents[0] >> 60) & 0xF);

		out[18] = '-';

		out[19] = hex_chr((contents[1] >>  0) & 0xF);
		out[20] = hex_chr((contents[1] >>  4) & 0xF);
		out[21] = hex_chr((contents[1] >>  8) & 0xF);
		out[22] = hex_chr((contents[1] >> 12) & 0xF);

		out[23] = '-';

		out[24] = hex_chr((contents[1] >> 16) & 0xF);
		out[25] = hex_chr((contents[1] >> 20) & 0xF);
		out[26] = hex_chr((contents[1] >> 24) & 0xF);
		out[27] = hex_chr((contents[1] >> 28) & 0xF);
		out[28] = hex_chr((contents[1] >> 32) & 0xF);
		out[29] = hex_chr((contents[1] >> 36) & 0xF);
		out[30] = hex_chr((contents[1] >> 40) & 0xF);
		out[31] = hex_chr((contents[1] >> 44) & 0xF);
		out[32] = hex_chr((contents[1] >> 48) & 0xF);
		out[33] = hex_chr((contents[1] >> 52) & 0xF);
		out[34] = hex_chr((contents[1] >> 56) & 0xF);
		out[35] = hex_chr((contents[1] >> 60) & 0xF);
		out[36] = 0;

		return std::string(out);
	}

	size_t UUIDHash::operator()(const UUID& p) const {
		std::hash<std::string> hsh;
		return hsh(p.as_string());
	}

}