#ifndef APP_COMPONENTS_NBTVIEW_SCHEMA_ANNOTATOR_HPP
#define APP_COMPONENTS_NBTVIEW_SCHEMA_ANNOTATOR_HPP

#include "../../../App.hpp"
#include "../../../ImGuiEx.hpp"
#include "../../../../nbt/Tag.hpp"
#include "../../../../nbt/TagCompound.hpp"
#include "../../../../nbt/TagList.hpp"
#include "../../../../util/Color.hpp"

namespace App::Components::NBTView::Schema {

	class Annotator {
		App* parent;

	public:
		Annotator(App* parent);
		
		bool renderHeaderIcon(const std::string& name, const char* type, ImGuiEx::Icon icon, ImGuiEx::Icon aux = ImGuiEx::Icon(), bool arr = false);

		void renderTag(const std::string& name, NBT::Tag* in);

		void renderCompound(NBT::TagCompound* in);

		void renderList(NBT::TagList* in);

		void begin();

		void end();

		App* getParent();
	};

}

#endif