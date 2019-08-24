#ifndef NBT_LOADER_HPP
#define NBT_LOADER_HPP

#include <string>

#include "Deserializer.hpp"
#include "TagRoot.hpp"

namespace NBT {

	class Loader {

		std::string path;
		TagRoot root_tag;

	public:
		Loader(std::string path_);

		void load();

		std::string& getPath();

		void unload();

		TagRoot& getRoot();
	};

}

#endif