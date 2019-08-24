#ifndef APP_COMPONENTS_WORLDVIEW_FRAGMENTSHADERS_HPP
#define APP_COMPONENTS_WORLDVIEW_FRAGMENTSHADERS_HPP

#include <glad/glad.h>

namespace App::Components::WorldView {

	class FragmentShaders {

		GLuint program_id = 0;

	public:
		FragmentShaders();

		GLuint getShaderID();

	};

}

#endif