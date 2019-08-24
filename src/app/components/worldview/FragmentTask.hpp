#ifndef APP_COMPONENTS_WORLDVIEW_FRAGMENTTASK_HPP
#define APP_COMPONENTS_WORLDVIEW_FRAGMENTTASK_HPP

#include "../../dispatchqueue/Task.hpp"
#include "Fragment.hpp"

namespace App::Components::WorldView {
	
	class FragmentTask : public DispatchQueue::Task {
	
	public:
		Fragment* current;

		FragmentTask(Fragment* use);

		void run();

	};

}

#endif