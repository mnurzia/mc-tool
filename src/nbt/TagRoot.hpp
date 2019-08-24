#ifndef NBT_TAGROOT_HPP
#define NBT_TAGROOT_HPP

#include "TagCompound.hpp"

namespace NBT {

	class TagRoot : public TagCompound {
		std::unique_ptr<TagCompound> root_tag;

	public:
		TagRoot();

		void load(Deserializer* stream);

		//void save(Serializer* stream);

		std::string asString();

		void clear();
	};

}

#endif