#include "Deserializer.hpp"

#include <cstdio>

#include <fstream>

namespace NBT {

	Deserializer::Deserializer(std::string filename) {
		input_file = std::ifstream(filename, std::ios::binary);
		input_buffer = new unsigned char[buffer_size];
		output_buffer = new unsigned char[buffer_size];
		zip_stream = { 0 };
		zip_stream.zalloc = NULL;
		zip_stream.zfree = NULL;
		zip_stream.opaque = NULL;
		zip_stream.avail_in = 0;
		zip_stream.next_in = &(input_buffer[0]);
		zip_stream.avail_out = 0;
		zip_stream.next_out = &(output_buffer[0]);
		size_t wptr = 0;
		int ret = inflateInit2(&zip_stream, 16 + MAX_WBITS);
		if (ret != Z_OK) {
			this->error("inflateInit2() returned '%i'.", ret);
		}
	}

	std::byte Deserializer::get() {
		if (output_buffer_pointer == output_buffer_size) {
			if (zip_stream.avail_in == 0) {
				input_file.read((char*)&(input_buffer[0]), buffer_size);
				std::streamsize amt_read = input_file.gcount();
				if (amt_read == 0) {
					this->error("Unexpected EOF.");
				}
				zip_stream.avail_in = (unsigned int)amt_read;
				zip_stream.next_in = &(input_buffer[0]);
			}
			zip_stream.avail_out = buffer_size;
			zip_stream.next_out = &(output_buffer[0]);
			int ret = inflate(&zip_stream, Z_NO_FLUSH);
			output_buffer_size = buffer_size - zip_stream.avail_out;
			output_buffer_pointer = 0;
			size_t wptr = 0;
		}
		return std::byte(output_buffer[output_buffer_pointer++]);
	}

	Deserializer::~Deserializer() {
		inflateEnd(&zip_stream);
		delete output_buffer;
		delete input_buffer;
	}
}