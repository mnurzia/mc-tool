#ifndef APP_COMPONENTS_UICOMPONENT_HPP
#define APP_COMPONENTS_UICOMPONENT_HPP

#include <spdlog/spdlog.h>

#include "../../util/UUID.hpp"

namespace App {

	class App;

	class UIComponent  {

		Util::UUID id;
		App* parent;
		std::string cls;

	protected:
		bool should_quit = true;
		std::shared_ptr<spdlog::logger> logger;

	public:

		UIComponent(App* parent, std::string cls);

		virtual void render();

		virtual void destroy();

		void quit();

		bool canQuit();

		std::string windowName(std::string name);

		Util::UUID getID();

		App* getParent();

		friend class App;
	};

}

#endif