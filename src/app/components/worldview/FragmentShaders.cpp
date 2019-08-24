#include "FragmentShaders.hpp"

#include <vector>

namespace App::Components::WorldView {

	static char const* FRAG_FRAG =
		"#version 450 core\n"
		"in vec2 frag_tex_coord;\n"
		"in vec4 frag_color;\n"
		"uniform sampler2D frag_texture;\n"
		"uniform sampler1D frag_palette;\n"
		"layout (location = 0) out vec4 final_color;\n"
		"void main() {\n"
		"    final_color = frag_color * texture(frag_palette, texture(frag_texture, frag_tex_coord).r);\n"
		"}\n";
	static char const* FRAG_VERT = 
		"#version 450 core\n"
		"layout (location = 0) in vec2 vert_pos;\n"
		"layout (location = 1) in vec2 vert_tex_coord;\n"
		"layout (location = 2) in vec4 vert_color;\n"
		"uniform mat4 proj_mtx;\n"
		"out vec2 frag_tex_coord;\n"
		"out vec4 frag_color;\n"
		"void main() {\n"
		"    frag_tex_coord = vert_tex_coord;\n"
		"    frag_color = vert_color;\n"
		"    gl_Position = proj_mtx * vec4(vert_pos.xy,0,1);\n"
		"}\n";

	FragmentShaders::FragmentShaders() {
		GLuint vert_shader_id = glCreateShader(GL_VERTEX_SHADER);
		GLuint frag_shader_id = glCreateShader(GL_FRAGMENT_SHADER);
		
		GLint result = GL_FALSE;
		int info_log_length = 0;
		glShaderSource(vert_shader_id, 1, &FRAG_VERT, NULL);
		glCompileShader(vert_shader_id);

		glGetShaderiv(vert_shader_id, GL_COMPILE_STATUS, &result);
		glGetShaderiv(vert_shader_id, GL_INFO_LOG_LENGTH, &info_log_length);

		if (info_log_length > 0) {
			printf("Vert Error!\n");
			std::vector<char> error_message(size_t(info_log_length) + 1);
			glGetShaderInfoLog(vert_shader_id, info_log_length, NULL, &error_message[0]);
			printf("%s\n", &error_message[0]);
			exit(1);
		}

		result = GL_FALSE;
		info_log_length = 0;
		glShaderSource(frag_shader_id, 1, &FRAG_FRAG, NULL);
		glCompileShader(frag_shader_id);

		glGetShaderiv(frag_shader_id, GL_COMPILE_STATUS, &result);
		glGetShaderiv(frag_shader_id, GL_INFO_LOG_LENGTH, &info_log_length);

		if (info_log_length > 0) {
			printf("Frag Error!\n");
			std::vector<char> error_message(size_t(info_log_length) + 1);
			glGetShaderInfoLog(frag_shader_id, info_log_length, NULL, &error_message[0]);
			printf("%s\n", &error_message[0]);
			exit(1);
		}

		program_id = glCreateProgram();
		glAttachShader(program_id, vert_shader_id);
		glAttachShader(program_id, frag_shader_id);
		glLinkProgram(program_id);
		glGetProgramiv(program_id, GL_LINK_STATUS, &result);
		glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &info_log_length);
		if (info_log_length > 0) {
			printf("Link Error!\n");
			std::vector<char> error_message(size_t(info_log_length) + 1);
			glGetProgramInfoLog(program_id, info_log_length, NULL, &error_message[0]);
			printf("%s\n", &error_message[0]);
			exit(1);
		}

		glDetachShader(program_id, vert_shader_id);
		glDetachShader(program_id, frag_shader_id);

		glDeleteShader(vert_shader_id);
		glDeleteShader(frag_shader_id);

		glUseProgram(program_id);
		glUniform1i(glGetUniformLocation(program_id, "frag_texture"), 0);
		glUseProgram(program_id);
		glUniform1i(glGetUniformLocation(program_id, "frag_palette"), 1);
	}

	GLuint FragmentShaders::getShaderID() {
		return program_id;
	}

}