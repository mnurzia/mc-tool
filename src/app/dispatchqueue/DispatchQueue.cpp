#include "DispatchQueue.hpp"

#include <string>

namespace App::DispatchQueue {

	DispatchQueue::DispatchQueue(size_t thread_count) : threads(thread_count), thread_state(thread_count) {
		for (size_t i = 0; i < threads.size(); i++) {
			threads[i] = std::thread(&DispatchQueue::thread_entry, this, int(i));
			thread_state[i] = DispatchQueue::RunState::IDLE;
		}
	}

	DispatchQueue::~DispatchQueue() {
		std::unique_lock<std::mutex> new_lock(lock);
		quit = true;
		new_lock.unlock();
		cond_var.notify_all();
		for (size_t i = 0; i < threads.size(); i++) {
			if (threads[i].joinable()) {
				threads[i].join();
			}
		}
	}

	void DispatchQueue::dispatch(std::unique_ptr<Task> task) {
		std::unique_lock<std::mutex> new_lock(lock);
		queue.push_back(std::move(task));
		queue.back().get()->onQueue();
		new_lock.unlock();
		cond_var.notify_all();
	}

	void DispatchQueue::thread_entry(int idx) {
		std::unique_lock<std::mutex> new_lock(lock);
		do {
			cond_var.wait(
				new_lock,
				[this] {
					return (queue.size() || quit);
				}
			);

			if (!quit && queue.size()) {
				std::unique_ptr<Task> task = std::move(queue.front());
				queue.pop_front();
				thread_state[idx] = DispatchQueue::RunState::RUNNING;
				task->onRun();
				new_lock.unlock();
				task->run();
				new_lock.lock();
			}
			thread_state[idx] = DispatchQueue::RunState::IDLE;

		} while (!quit);
	}

	std::tuple<int, int, int> DispatchQueue::getComposition() {
		int init = 0;
		int queued = 0;
		int running = 0;
		std::unique_lock<std::mutex> new_lock(lock);
		queued = int(queue.size());
		for (int i = 0; i < threads.size(); i++) {
			if (thread_state[i] == RunState::RUNNING) {
				running++;
			}
		}
		new_lock.unlock();
		return std::tuple<int, int, int>(init, queued, running);
	}

	void DispatchQueue::groupAdd(Util::UUID id, TaskGroup* group) {
		groups[id] = group;
	}

	void DispatchQueue::groupClear(Util::UUID id) {
		groups.erase(id);
	}

	std::unordered_map<Util::UUID, TaskGroup*, Util::UUIDHash>& DispatchQueue::getGroups() {
		return groups;
	}

}