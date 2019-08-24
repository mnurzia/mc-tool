#include "TaskGroup.hpp"

namespace App::DispatchQueue {

	void TaskGroup::setName(std::string name_) {
		name = name_;
	}

	std::string TaskGroup::getName() {
		return name;
	}

	void TaskGroup::taskAdd(Util::UUID id) {
		std::unique_lock<std::mutex> new_lock(lock);
		statuses[id] = Task::RunState::INITIALIZED;
		upper++;
		new_lock.unlock();
	}

	void TaskGroup::taskSet(Util::UUID id, Task::RunState state) {
		statuses[id] = state;
	}

	void TaskGroup::taskClear(Util::UUID id) {
		statuses.erase(id);
		processed++;
		if (statuses.size() == 0) {
			processed = 0;
			upper = 0;
		}
	}

	int TaskGroup::getUpper() {
		return upper;
	}

	int TaskGroup::getProcessed() {
		return processed;
	}

	std::tuple<int, int, int> TaskGroup::getComposition() {
		int init = 0;
		int queued = 0;
		int running = 0;
		std::unique_lock<std::mutex> new_lock(lock);
		for (auto el : statuses) {
			switch (el.second) {
				case Task::RunState::INITIALIZED:
					init++;
					break;
				case Task::RunState::QUEUED:
					queued++;
					break;
				case Task::RunState::RUNNING:
					running++;
					break;
			}
		}
		new_lock.unlock();
		return std::tuple<int, int, int>(init, queued, running);
	}

}