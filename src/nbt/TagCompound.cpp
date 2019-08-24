#include "TagCompound.hpp"

#include "TagByte.hpp"
#include "TagByteArray.hpp"
#include "TagDouble.hpp"
#include "TagFloat.hpp"
#include "TagInt.hpp"
#include "TagIntArray.hpp"
#include "TagList.hpp"
#include "TagLong.hpp"
#include "TagLongArray.hpp"
#include "TagShort.hpp"
#include "TagString.hpp"

namespace NBT {

	TagCompound::TagCompound() {
		type = Tag::Type::COMPOUND;
	}

	void TagCompound::load(Deserializer* stream) {
		Tag::Type current_type = Tag::Type::END;
		while ((current_type = (Tag::Type)stream->get()) != Tag::Type::END) {
			std::unique_ptr<Tag> current_tag;
			switch (current_type) {
				case Tag::Type::BYTE:
					current_tag = std::make_unique<TagByte>();
					break;
				case Tag::Type::SHORT:
					current_tag = std::make_unique<TagShort>();
					break;
				case Tag::Type::INT:
					current_tag = std::make_unique<TagInt>();
					break;
				case Tag::Type::LONG:
					current_tag = std::make_unique<TagLong>();
					break;
				case Tag::Type::FLOAT:
					current_tag = std::make_unique<TagFloat>();
					break;
				case Tag::Type::DOUBLE:
					current_tag = std::make_unique<TagDouble>();
					break;
				case Tag::Type::BYTE_ARRAY:
					current_tag = std::make_unique<TagByteArray>();
					break;
				case Tag::Type::STRING:
					current_tag = std::make_unique<TagString>();
					break;
				case Tag::Type::LIST:
					current_tag = std::make_unique<TagList>();
					break;
				case Tag::Type::COMPOUND:
					current_tag = std::make_unique<TagCompound>();
					break;
				case Tag::Type::INT_ARRAY:
					current_tag = std::make_unique<TagIntArray>();
					break;
				case Tag::Type::LONG_ARRAY:
					current_tag = std::make_unique<TagLongArray>();
					break;
				default:
					stream->error("Invalid type for compound tag: '%i'", current_type);
					break;
			}
			std::string current_name;
			uint16_t name_len = (((uint16_t)stream->get()) << 8) + (uint16_t)stream->get();
			for (uint16_t i = 0; i < name_len; i++) {
				current_name.append(1, (char)stream->get());
			}
			values[current_name] = std::move(current_tag);
			stream->pushName(current_name);
			values[current_name].get()->load(stream);
			stream->popName();
		}
	}

	void TagCompound::save(Serializer* stream) {

	}

	std::string TagCompound::asString() {
		std::string out = "{";
		int i = 0;
		for (auto it = values.begin(); it != values.end(); ++it) {
			if (i != 0) {
				out += ",";
				i++;
			}
			for (auto ch : it->first) {
				if (ch == '"') {
					out += "\\\"";
				}
				else {
					out += ch;
				}
			}
			out += ":";
			out += it->second.get()->asString();
			i++;
		}
		out += "}";
		return out;
	}
	
	Tag* TagCompound::getValue(std::string key) {
		return values[key].get();
	}

	void TagCompound::clear() {
		values.clear();
	}

	TagCompound::Iterator TagCompound::begin() {
		return values.begin();
	}

	TagCompound::Iterator TagCompound::end() {
		return values.end();
	}

	size_t TagCompound::entries() {
		return values.size();
	}

	size_t TagCompound::size() {
		return values.size();
	}

}