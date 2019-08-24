#ifndef APP_COMPONENTS_NBTVIEW_NBTVIEW_HPP
#define APP_COMPONENTS_NBTVIEW_NBTVIEW_HPP

#include <string>

#include "../WindowComponent.hpp"
#include "../../../nbt/Loader.hpp"
#include "schema/Annotator.hpp"

namespace App::Components::NBTView {

	class NBTView : public WindowComponent {
		NBT::Loader loader;
		Schema::Annotator annotator;

	public:

		NBTView(App* parent_, std::string path);

		void renderWin();
	};

}

#endif