#ifndef APP_RESOURCE_MANAGER_HPP
#define APP_RESOURCE_MANAGER_HPP

#include "../App.hpp"

namespace App::Resource {

	class Handle;

	class Manager {

	public:
		Manager(App* parent);

		Handle getTexHandle();
	};

}

#endif