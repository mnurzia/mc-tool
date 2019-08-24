#ifndef APP_RESOURCE_CONTAINER_CONTAINER_HPP
#define APP_RESOURCE_CONTAINER_CONTAINER_HPP

#include "../provider/Provider.hpp"

namespace App::Resource::Container {

	class Container {
		bool ready = false;

	public:
		Container(Provider* parent);

	};

}

#endif