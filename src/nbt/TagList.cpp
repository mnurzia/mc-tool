#include "TagList.hpp"

#include "TagByte.hpp"
#include "TagByteArray.hpp"
#include "TagCompound.hpp"
#include "TagDouble.hpp"
#include "TagFloat.hpp"
#include "TagInt.hpp"
#include "TagIntArray.hpp"
#include "TagLong.hpp"
#include "TagLongArray.hpp"
#include "TagShort.hpp"
#include "TagString.hpp"

namespace NBT {

	TagList::TagList() {
		type = Tag::Type::LIST;
	}

	void TagList::load(Deserializer* stream) {
		Tag::Type list_type = (Tag::Type)stream->get();
		num = ((int32_t)stream->get() << 24) +
			   ((int32_t)stream->get() << 16) +
			   ((int32_t)stream->get() <<  8) +
			   ((int32_t)stream->get());
		if (num <= 0 && list_type != Tag::Type::END) {
			stream->error("Expected END tag for size less than zero, but got '%i' instead.", type);
		}
		if (list_type != Tag::Type::END) {
			for (int32_t i = 0; i < num; i++) {
				std::unique_ptr<Tag> current_tag;
				switch (list_type) {
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
						stream->error("Invalid type for compound tag: '%i'", type);
						break;
				}
				values.push_back(std::move(current_tag));
				std::string cur_name = "[" + std::to_string(i) + "]";
				stream->pushName(cur_name);
				values[i]->load(stream);
				stream->popName();
			}
		}
	}

	void TagList::save(Serializer* stream) {

	}

	std::string TagList::asString() {
		std::string out = "[";
		int i = 0;
		for (auto& el : values) {
			if (i != 0) {
				out += ",";
			}
			out += el.get()->asString();
			i++;
		}
		out += "]";
		return out;
	}

	TagList::Iterator TagList::begin() {
		return values.begin();
	}

	TagList::Iterator TagList::end() {
		return values.end();
	}

	size_t TagList::items() {
		return values.size();
	}

	size_t TagList::size() {
		return values.size();
	}

	size_t TagList::alloc() {
		return num;
	}

	Tag::Type TagList::getListType() {
		return list_type;
	}
}