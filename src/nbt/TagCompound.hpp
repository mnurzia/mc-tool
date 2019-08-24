#ifndef NBT_TAGCOMPOUND_HPP
#define NBT_TAGCOMPOUND_HPP

#include <parallel_hashmap/phmap.h>

#include "Tag.hpp"

namespace NBT {

	class TagCompound : public Tag {
	protected:
		phmap::flat_hash_map<std::string, std::unique_ptr<Tag>> values;

		typedef phmap::flat_hash_map<std::string, std::unique_ptr<Tag>>::iterator Iterator;

	public:
		TagCompound();

		void load(Deserializer* stream);

		void save(Serializer* stream);

		std::string asString();

		Tag* getValue(std::string key);

		void clear();

		Iterator begin();

		Iterator end();

		size_t entries();

		size_t size();
	};

}

#endif