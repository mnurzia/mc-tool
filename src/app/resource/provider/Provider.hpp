#ifndef APP_RESOURCE_PROVIDER_PROVIDER_HPP
#define APP_RESOURCE_PROVIDER_PROVIDER_HPP

#include <parallel_hashmap/phmap.h>

#include "../../../util/UUID.hpp"
#include "../handle/Handle.hpp"

namespace App::Resource::Provider {
	class Provider {
		phmap::flat_hash_map<Util::UUID, std::unique_ptr<Container>> resources;

	public:
		Provider();

	};
}

#endif