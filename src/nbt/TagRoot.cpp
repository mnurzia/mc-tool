#include "TagRoot.hpp"

namespace NBT {

	TagRoot::TagRoot() {
		type = Tag::Type::COMPOUND;
	}

	void TagRoot::load(Deserializer* stream) {
		root_tag = std::make_unique<TagCompound>();
		Tag::Type current_type = Tag::Type::END;
		current_type = (Tag::Type)stream->get();
		if (current_type != Tag::Type::COMPOUND) {
			stream->error("Expected compound for root tag.");
		} else {
			std::string current_name;
			uint16_t name_len = (((uint16_t)stream->get()) << 8) + (uint16_t)stream->get();
			for (uint16_t i = 0; i < name_len; i++) {
				current_name.append(1, (char)stream->get());
			}
			root_tag.get()->load(stream);
		}
		values["Root Tag"] = std::move(root_tag);
	}

	std::string TagRoot::asString() {
		return values["Root Tag"].get()->asString();
	}

	void TagRoot::clear() {
		root_tag.reset();
		values.clear();
	}

}