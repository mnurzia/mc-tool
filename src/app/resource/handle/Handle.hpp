#ifndef APP_RESOURCE_HANDLE_HANDLE_HPP
#define APP_RESOURCE_HANDLE_HANDLE_HPP

#include "../../../util/UUID.hpp"

namespace App::Resource::Handle {

	class Handle {
		Util::UUID id;

	public:
		Handle(Provider* parent);

		Util::UUID getID();

	};

}

#endif