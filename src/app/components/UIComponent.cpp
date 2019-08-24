#include "UIComponent.hpp"

#include <typeinfo>

#include "../App.hpp"

namespace App {
	UIComponent::UIComponent(App* parent_, std::string cls) : parent(parent_) {
		logger = parent->getLogger("app." + cls);
	}

	void UIComponent::render() {

	}

	void UIComponent::destroy() {

	}

	void UIComponent::quit() {
		should_quit = false;
	}

	bool UIComponent::canQuit() {
		return !should_quit;
	}

	std::string UIComponent::windowName(std::string name) {
		return name + "##" + id.as_string();
	}

	Util::UUID UIComponent::getID() {
		return id;
	}

	App* UIComponent::getParent() {
		return parent;
	}
}