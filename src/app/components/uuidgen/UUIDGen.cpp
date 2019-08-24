#include "UUIDGen.hpp"

#include "../../ImGuiEx.hpp"

namespace App::Components::UUIDGen {

	UUIDGen::UUIDGen(App* app) : WindowComponent(app, "UUID Generator", "uuidgen") {

	}

	void UUIDGen::renderWin() {
		if (!generated) {
			current = Util::UUID();
			current_string = current.as_string();
			generated = true;
		}
		ImGuiEx::SelectableFieldCode(current_string.c_str());
		if (ImGui::Button("Generate v4 UUID")) {
			generated = false;
		}
		ImGui::SameLine();
		ImGuiEx::HelpMarker("Click the button to generate a random 128-bit number similarly to how Minecraft does.");
	}

}