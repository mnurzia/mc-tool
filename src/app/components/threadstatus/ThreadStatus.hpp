#ifndef APP_COMPONENTS_THREADSTATUS_THREADSTATUS_HPP
#define APP_COMPONENTS_THREADSTATUS_THREADSTATUS_HPP

#include "../UIComponent.hpp"

namespace App::Components::ThreadStatus {

	class ThreadStatus : public UIComponent {

		float decay = 0.0f;
		float prog = 0.0f;
		float prog_next = 0.0f;

	public:

		ThreadStatus(App* parent_);

		void render();

	};

}

#endif