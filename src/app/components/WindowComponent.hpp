#ifndef APP_COMPONENTS_WINDOWCOMPONENT_HPP
#define APP_COMPONENTS_WINDOWCOMPONENT_HPP

#include <imgui/imgui.h>

#include "../App.hpp"
#include "UIComponent.hpp"

namespace App::Components {

	class WindowComponent : public UIComponent {
		std::string comp_win_name;
		ImGuiWindowFlags win_flags = 0;
		bool* closed = NULL;
		ImVec2 size;
		bool first = true;

	public:
		WindowComponent(App* parent_, std::string win_name, std::string cls);

		std::string& getWinName();
		
		void setWinFlags(ImGuiWindowFlags in);

		void setClosed(bool* closed);

		void setSize(ImVec2 in);
		
		virtual void render();

		virtual void renderWin();

		bool isFirst();

	};

}

#endif