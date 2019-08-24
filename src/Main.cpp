#include "app/App.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

int main() {
	App::App app;
	app.init();
	app.run();
	app.destroy();
	//MCFunction::Parser parser;
	//std::vector<MCFunction::Token> toks;
	//try {
	//	parser.parse("execute if entity @s[tag=!mcscriptStop] if entity @s[tag=loop] run function examples:mcscript/while6 {\"hello world\"}");
	//	parser.tokenizeArgs(toks);
	//} catch (MCFunction::ParserException e) {
	//	printf("%s\n", e.what());
	//}
	//for (auto arg : toks) {
	//	//("%s %i %i\n", arg.getContents().c_str(), arg.getPos(), arg.getLen());
	//}
	//MCFunction::Node::Root root_node;
	//MCFunction::Node::BaseCommand time_cmd(&root_node, "time");
	//MCFunction::Node::StringUnion time_subcmd(&time_cmd);
	//MCFunction::Node::Integer time_set_value(&time_subcmd, "value");
	//root_node.addCommand("time", &time_cmd);
	//time_cmd.setArg(&time_subcmd);
	//time_subcmd.addValue("set", &time_set_value);
	//time_subcmd.addValue("query", &time_set_value);
	//time_subcmd.addValue("add", &time_set_value);
	//MCFunction::Node::Node* current = &root_node;
	//const char* cmd = "time set 0";
	//size_t offset = 0;
	//size_t last_offset = 0;
	//size_t length_total = strlen(cmd);
	//std::vector<MCFunction::Suggestion> suggs;
	//for (size_t i = 0; i < length_total; i++) {
	//	offset = 0;
	//	last_offset = 0;
	//	current = &root_node;
	//	bool break_now = false;
	//	while (current != nullptr && !break_now) {
	//		suggs.clear();
	//		try {
	//			//printf("%s ", current->getCanonicalName().c_str());
	//			last_offset = offset;
	//			current = current->next(cmd, &offset, i);
	//			printf("%llu %llu %.*s\n", last_offset, offset, int(offset - last_offset), &(cmd[last_offset]));
	//		} catch (MCFunction::Node::Exception e) {
	//			//printf("\n%s\n", e.what());
	//			break_now = true;
	//		}
	//		current->getSuggestions(cmd, last_offset, offset - last_offset, suggs);
	//		for (auto sugg : suggs) {
	//			//printf("(%s, %s) ", sugg.getValue().c_str(), sugg.getDescription().c_str());
	//		}
	//	}
	//}
	//printf("\n");
	return 0;
}

//int main() {
//	std::ifstream ifile("assets/test.dat", std::ios::binary);
//	unsigned char* in_buf = new unsigned char[512 * 1024];
//	unsigned char* out_buf = new unsigned char[512 * 1024];
//	z_stream in_stream = { 0 };
//	in_stream.zalloc = NULL;
//	in_stream.zfree = NULL;
//	in_stream.opaque = NULL;
//	in_stream.avail_in = 0;
//	in_stream.next_in = in_buf;
//	in_stream.avail_out = 0;
//	in_stream.next_out = out_buf;
//	size_t wptr = 0;
//	int ret = inflateInit2(&in_stream, 16 + MAX_WBITS);
//	if (ret != Z_OK) {
//		printf("ERR!");
//		exit(1);
//	}
//	while (1) {
//		if (in_stream.avail_in == 0) {
//			ifile.read((char*)in_buf, 512);
//			std::streamsize amt_read = ifile.gcount();
//			if (amt_read == 0) {
//				break;
//			}
//			in_stream.avail_in = (unsigned int)amt_read;
//			in_stream.next_in = in_buf;
//			printf("Filled!\n");
//		}
//		in_stream.avail_out = 512;
//		in_stream.next_out = out_buf;
//		int ret = inflate(&in_stream, Z_NO_FLUSH);
//		int bytes_in_buf = 512 - in_stream.avail_out;
//		printf("%i %i %i %i\n", ret, in_stream.avail_in, in_stream.avail_out, bytes_in_buf);
//		//for (int i = 0; i < amt_read; i++) {
//		//	if (wptr % 32 == 0) {
//		//		printf("%08X | ", wptr);
//		//	}
//		//	printf("%02X ", out_buf[i]);
//		//	if (wptr % 32 == 31) {
//		//		printf("\n");
//		//	}
//		//	wptr++;
//		//}
//		//printf("Looped!\n");
//	}
//	inflateEnd(&in_stream);
//}