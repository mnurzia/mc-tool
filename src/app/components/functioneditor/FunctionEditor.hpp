#ifndef APP_COMPONENTS_FUNCTIONEDITOR_HPP
#define APP_COMPONENTS_FUNCTIONEDITOR_HPP

#include "../WindowComponent.hpp"

#include "imgui_textedit/imgui_textedit.h"

namespace App::Components::FunctionEditor {

	class FunctionEditor : public WindowComponent {
		TextEditor editor;
		TextEditor::LanguageDefinition language;

	public:
		FunctionEditor(App* app);

		void renderWin();

	};

}

#endif