#include "Tag.hpp"

namespace NBT {

	const char* TagNames[] = {
		"TAG_End",
		"TAG_Byte",
		"TAG_Short",
		"TAG_Int",
		"TAG_Long",
		"TAG_Float",
		"TAG_Double",
		"TAG_Byte_Array",
		"TAG_String",
		"TAG_List",
		"TAG_Compound",
		"TAG_Int_Array",
		"TAG_Long_Array"
	};

	Tag::Type Tag::getType() {
		return type;
	}

	void Tag::load(Deserializer* stream) {

	}

	void Tag::save(Serializer* stream) {

	}

	std::string Tag::asString() {
		return "";
	}

}