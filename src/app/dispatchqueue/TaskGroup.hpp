#ifndef APP_DISPATCHQUEUE_TASKGROUP_HPP
#define APP_DISPATCHQUEUE_TASKGROUP_HPP

#include <mutex>
#include <tuple>
#include <unordered_map>

#include "Task.hpp"

namespace App::DispatchQueue {

	class TaskGroup {

		std::unordered_map<Util::UUID, Task::RunState, Util::UUIDHash> statuses;
		std::string name;
		std::mutex lock;
		Util::UUID id;
		int upper = 0;
		int processed = 0;

	public:
		void setName(std::string name_);

		std::string getName();

		void taskAdd(Util::UUID id);

		void taskSet(Util::UUID id, Task::RunState runstate);

		void taskClear(Util::UUID id);

		int getUpper();

		int getProcessed();

		std::tuple<int, int, int> getComposition();
	};

}

#endif