#ifndef APP_DISPATCHQUEUE_DISPATCHQUEUE_HPP
#define APP_DISPATCHQUEUE_DISPATCHQUEUE_HPP

#include <memory>
#include <mutex>
#include <queue>
#include <thread>
#include <unordered_map>
#include <vector>

#include "Task.hpp"
#include "TaskGroup.hpp"

// Excellent reference:
// https://github.com/embeddedartistry/embedded-resources/blob/master/examples/cpp/dispatch.cpp

namespace App::DispatchQueue {

	class DispatchQueue {

	public:

		enum class RunState {
			IDLE,
			RUNNING
		};

		/*
		 * @brief Constructor
		 *
		 * @param thread_count The number of threads to use.
		 */
		DispatchQueue(size_t thread_count = 15);

		/*
		 * @brief Destructor
		 */
		~DispatchQueue();

		/*
		 * @brief Main dispatch function and interface
		 *
		 * @param task The Task to be executed.
		 */
		void dispatch(std::unique_ptr<Task> task);

		/**
		 * @brief Get the amount of initialized tasks, queued tasks, and
		 *        running tasks.
		 */
		std::tuple<int, int, int> getComposition();

		/**
		 * @brief Adds a group with id &c id.
		 */
		void groupAdd(Util::UUID id, TaskGroup* group);
		
		/**
		 * @brief Delete group with id @c id.
		 */
		void groupClear(Util::UUID id);

		/**
		 * @brief Gets all groups.
		 */
		std::unordered_map<Util::UUID, TaskGroup*, Util::UUIDHash>& getGroups();

	private:
		std::mutex lock;
		std::vector<std::thread> threads;
		std::vector<RunState> thread_state;
		std::deque<std::unique_ptr<Task>> queue;
		std::condition_variable cond_var;
		std::unordered_map<Util::UUID, TaskGroup*, Util::UUIDHash> groups;
		bool quit = false;

		void thread_entry(int idx);
	};

}

#endif