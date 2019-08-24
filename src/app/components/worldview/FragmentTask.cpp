#include "FragmentTask.hpp"

#include <cstdio>

#include <string>

#include "FragmentManager.hpp"

namespace App::Components::WorldView {

	FragmentTask::FragmentTask(Fragment* use):DispatchQueue::Task(use->getParent()->getGroup()) {
		current = use;
		setName("Fragment Generator<seed=" + std::to_string(current->getGenerator().getSeed()) + ">");
	}

	void FragmentTask::run() {
		setStatus("Generating biomes");
		current->generate();
		current->setState(Fragment::State::GENERATED);
	}

}