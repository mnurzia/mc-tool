#include "../PersistentWindowComponent.hpp"

namespace App::Components::LogView {

	class LogView : public PersistentWindowComponent {

	public:
		LogView(App* parent);

		void renderWin();

	};

}