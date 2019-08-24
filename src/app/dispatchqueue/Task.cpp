#include "Task.hpp"

#include <chrono>
#include <thread>

#include "TaskGroup.hpp"

namespace App::DispatchQueue {

	Task::Task(TaskGroup& parent_) : parent(parent_) {
		parent.taskAdd(id);
	}

	Task::~Task() {
		parent.taskClear(id);
	}

	void Task::onQueue() {
		parent.taskSet(id, Task::RunState::QUEUED);
	}

	void Task::onRun() {
		parent.taskSet(id, Task::RunState::RUNNING);
	}

	void Task::setName(std::string name_) {
		name = name;
	}

	std::string Task::getName() {
		return name;
	}

	void Task::setStatus(std::string status_) {
		status = status_;
	}
	
	std::string Task::getStatus() {
		return status;
	}

	void Task::run() {
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		printf("Test Task!\n");
	}

}