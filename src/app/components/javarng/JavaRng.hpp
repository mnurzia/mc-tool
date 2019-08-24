#ifndef APP_COMPONENTS_JAVARNG_JAVARNG_HPP
#define APP_COMPONENTS_JAVARNG_JAVARNG_HPP

#include "../WindowComponent.hpp"

namespace App::Components::JavaRNG {

	class JavaRNG : public WindowComponent {
		int64_t current_seed;
		int32_t int_bound = 0;

	public:
		JavaRNG(App* app);

		void renderWin();

	};

}

#endif