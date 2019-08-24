#ifndef APP_COMPONENTS_UUIDGEN_HPP
#define APP_COMPONENTS_UUIDGEN_HPP

#include "../../../util/UUID.hpp"

#include "../WindowComponent.hpp"

namespace App::Components::UUIDGen {

	class UUIDGen : public WindowComponent {
		bool generated = false;
		Util::UUID current;
		std::string current_string;

	public:
		UUIDGen(App* app);

		void renderWin();

	};

}

#endif