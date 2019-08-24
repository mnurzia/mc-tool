#ifndef NBT_TAGLIST_HPP
#define NBT_TAGLIST_HPP

#include <memory>
#include <vector>

#include "Tag.hpp"

namespace NBT {

	class TagList : public Tag {
		Tag::Type list_type = Tag::Type::END;

		int32_t num = 0;

		std::vector<std::unique_ptr<Tag>> values;
		typedef std::vector<std::unique_ptr<Tag>>::iterator Iterator;

	public:
		TagList();

		void load(Deserializer* stream);

		void save(Serializer* stream);

		std::string asString();

		Iterator begin();

		Iterator end();

		size_t items();

		size_t size();

		size_t alloc();

		Tag::Type getListType();
	};

}

#endif