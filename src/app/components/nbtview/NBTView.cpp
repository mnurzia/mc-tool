#include "NBTView.hpp"

#include <imgui/imgui.h>

#include "../../ImGuiEx.hpp"

namespace App::Components::NBTView {

	NBTView::NBTView(App* parent_, std::string path) : WindowComponent(parent_, "NBT Viewer - " + path, "nbtview"), loader(path), annotator(parent_) {
		loader.load();
	}

	void NBTView::renderWin() {
		if (ImGui::Button("Refresh")) {
			loader.load();
			loader.unload();
		}
		NBT::TagRoot& root = loader.getRoot();
		ImGui::Text(loader.getPath().c_str());
		if (ImGui::BeginChild(ImGui::GetID("Editor"), ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar)) {
			annotator.begin();
			annotator.renderCompound(&root);
			annotator.end();
			ImGui::EndChild();
		}
	}

}