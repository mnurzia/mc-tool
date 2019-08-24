#ifndef NBT_SERIALBASE_HPP
#define NBT_SERIALBASE_HPP

#include <string>
#include <vector>

namespace NBT {

	class SerialBase {
		std::vector<std::string> name_stack;

	public:
		void pushName(std::string& name);

		void popName();

		std::vector<std::string>& getNameStack();

		void error(const char* fmt, ...);

	};

}

#endif