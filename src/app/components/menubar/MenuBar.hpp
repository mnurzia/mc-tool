#ifndef APP_COMPONENTS_MENUBAR_MENUBAR_HPP
#define APP_COMPONENTS_MENUBAR_MENUBAR_HPP

#include "../UIComponent.hpp"

namespace App::Components::MenuBar {
	
	class MenuBar : public UIComponent {

		bool dark_mode = true;

	public:
		MenuBar(App* parent_);

		void render();

	};

}

#endif