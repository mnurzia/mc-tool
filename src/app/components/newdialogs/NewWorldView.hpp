#ifndef APP_COMPONENTS_NEWDIALOGS_NEWWORLDVIEW_HPP
#define APP_COMPONENTS_NEWDIALOGS_NEWWORLDVIEW_HPP

#include <cstdint>

#include <string>

#include "../UIComponent.hpp"

namespace App::Components::NewDialogs {

	class NewWorldView : public UIComponent {

		std::string win_name;

		int64_t seed;

		bool should_proceed;

		char seed_input[22] = "\0";

		int64_t parseSeed(char* in, int* type);

	public:
		NewWorldView(App* parent_);

		void render();

	};

}

#endif