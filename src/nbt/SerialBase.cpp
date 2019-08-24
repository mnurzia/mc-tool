#include "SerialBase.hpp"

#include <varargs.h>

namespace NBT {

	void SerialBase::pushName(std::string& name) {
		name_stack.push_back(name);
	}

	void SerialBase::popName() {
		name_stack.pop_back();
	}

	std::vector<std::string>& SerialBase::getNameStack() {
		return name_stack;
	}

	void SerialBase::error(const char* fmt, ...) {
		printf("Error at ");
		for (int i = 0; i < name_stack.size(); i++) {
			if (i == 0) {
				printf("%s", name_stack[i].c_str());
			} else {
				printf(".%s", name_stack[i].c_str());
			}
		}
		printf(": ");
		va_list args;
		va_start(args);
		vprintf(fmt, args);
		va_end(args);
		exit(1);
	}
}