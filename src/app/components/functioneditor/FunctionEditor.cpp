#include "FunctionEditor.hpp"

#include <fstream>

#include "../../ImGuiEx.hpp"
#include "LangDef.hpp"

namespace App::Components::FunctionEditor {

	FunctionEditor::FunctionEditor(App* app) : WindowComponent(app, "Text Editor", "functioneditor") {
		language = FunctionLangDef();
		editor.SetLanguageDefinition(language);
		editor.SetPalette(TextEditor::GetDarkPalette());
		static const char* fileToEdit = "assets/test.mcfunction"; {
			std::ifstream t(fileToEdit);
			if (t.good()) {
				std::string str((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
				editor.SetText(str);
			}
		}
	}

	void FunctionEditor::renderWin() {
		ImGuiEx::BeginCode();
		editor.Render("TextEditor");
		ImGuiEx::EndCode();
	}

}