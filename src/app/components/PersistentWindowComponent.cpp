#include "PersistentWindowComponent.hpp"

namespace App::Components {

	PersistentWindowComponent::PersistentWindowComponent(App* parent_, std::string win_name, std::string cls) : WindowComponent(parent_, win_name, cls) {
		setClosed(&win_opened);
	}

	bool PersistentWindowComponent::getOpened() {
		return win_opened;
	}

	void PersistentWindowComponent::setOpened(bool opened_) {
		win_opened = opened_;
	}

}