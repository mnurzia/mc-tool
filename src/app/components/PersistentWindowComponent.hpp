#ifndef APP_COMPONENTS_PERSISTENTWINDOWCOMPONENT_HPP
#define APP_COMPONENTS_PERSISTENTWINDOWCOMPONENT_HPP

#include <imgui/imgui.h>

#include "../App.hpp"
#include "WindowComponent.hpp"

namespace App::Components {

	class PersistentWindowComponent : public WindowComponent {
		bool win_opened = false;

	public:
		PersistentWindowComponent(App* parent_, std::string win_name, std::string cls);

		bool getOpened();

		void setOpened(bool opened_);
	};

}

#endif