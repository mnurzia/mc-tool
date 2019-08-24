#include "Loader.hpp"

namespace NBT {

	Loader::Loader(std::string path_) {
		path = path_;
	}

	void Loader::load() {
		Deserializer deser(path);
		root_tag.load(&deser);
	}

	void Loader::unload() {
		root_tag.clear();
	}

	std::string& Loader::getPath() {
		return path;
	}

	TagRoot& Loader::getRoot() {
		return root_tag;
	}

}