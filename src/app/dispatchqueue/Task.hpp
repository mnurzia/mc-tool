#ifndef APP_DISPATCHQUEUE_TASK_HPP
#define APP_DISPATCHQUEUE_TASK_HPP

#include <mutex>
#include <string>

#include"../../util/UUID.hpp"

namespace App::DispatchQueue {

	class TaskGroup;

	class Task {

	public:
		enum class RunState {
			INITIALIZED,
			QUEUED,
			RUNNING
		};

	private:

		std::string name;
		std::string status;
		RunState run_state = RunState::INITIALIZED;
		Util::UUID id;
		TaskGroup& parent;
		std::mutex lock;

	public:
		Task(TaskGroup& parent_);

		~Task();

		void onQueue();

		void onRun();

		void setName(std::string name_);

		std::string getName();

		void setStatus(std::string status_);

		std::string getStatus();

		virtual void run();

	};

}

#endif