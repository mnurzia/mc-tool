#ifndef NBT_DESERIALIZER_HPP
#define NBT_DESERIALIZER_HPP

#include <cstddef>

#include <fstream>
#include <string>
#include <vector>

#include <zlib/zlib.h>

#include "SerialBase.hpp"

namespace NBT {

	class Deserializer : public SerialBase {

		std::ifstream input_file;
		z_stream zip_stream;
		int buffer_size = 512 * 1024;
		size_t output_buffer_pointer = 0;
		size_t output_buffer_size = 0;
		unsigned char* input_buffer;
		unsigned char* output_buffer;

	public:
		Deserializer(std::string filename);
		~Deserializer();

		std::byte get();

	};

}

#endif