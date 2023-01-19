#include "MenuBar.hpp"

#include <memory>

#include <imgui/imgui.h>
#include <pastel_icons.h>

#include "../../App.hpp"
#include "../../ImGuiEx.hpp"
#include "../newdialogs/NewWorldView.hpp"
#include "../fontmanager/FontManager.hpp"
#include "../uuidgen/UUIDGen.hpp"
#include "../javarng/JavaRng.hpp"
#include "../logview/LogView.hpp"

namespace App::Components::MenuBar {

	MenuBar::MenuBar(App* parent_) : UIComponent(parent_, "menubar") {

	}

	void MenuBar::render() {
		if (ImGui::BeginMainMenuBar()) {
			if (ImGui::BeginMenu("File")) {
				ImGuiEx::MenuIcon(ICON_PA_PAGE_ADD);
				if (ImGui::MenuItem("New", "CTRL-N")) {
					//std::unique_ptr<NewDialogs::NewWorldView> out(new NewDialogs::NewWorldView(getParent()));
					getParent()->addComponent(
						std::unique_ptr<UIComponent>(new NewDialogs::NewWorldView(getParent()))
					);
				}
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("View")) {
				ImGuiEx::MenuIcon(ICON_PA_CONTRAST);
				if (ImGui::MenuItem("Dark Mode", NULL, &dark_mode)) {
					if (dark_mode) {
						ImGui::StyleColorsDark();
					} else {
						ImGui::StyleColorsLight();
					}
				}
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Tools")) {
				if (ImGui::BeginMenu("Debug")) {
					bool open = getParent()->getFontManager()->getOpened();
					if (ImGui::MenuItem("Font Manager", NULL, &open)) {
						getParent()->getFontManager()->setOpened(open);
					}
					open = getParent()->getLogView()->getOpened();
					if (ImGui::MenuItem("Log Viewer", NULL, &open)) {
						getParent()->getLogView()->setOpened(open);
					}
					ImGui::EndMenu();
				}
				if (ImGui::MenuItem("UUID Generator")) {
					getParent()->addComponent(
						std::unique_ptr<UIComponent>(new UUIDGen::UUIDGen(getParent()))
					);
				}
				if (ImGui::MenuItem("Java RNG")) {
					getParent()->addComponent(
						std::unique_ptr<UIComponent>(new JavaRNG::JavaRNG(getParent()))
					);
				}
				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}

	}

}
