#ifndef UTIL_COLOR_HPP
#define UTIL_COLOR_HPP

#include <cstdint>

#include <glm/vec4.hpp>
#include <imgui/imgui.h>

namespace Util {

	class Color {

	public:
		uint8_t r, g, b, a;

		Color(uint8_t r = 0, uint8_t g = 0, uint8_t b = 0, uint8_t a = 255);
		Color(uint8_t l, uint8_t a = 255);
		Color(uint32_t rgba);
		Color(uint8_t* rgb, uint8_t a);

		Color operator+(const Color& c);
		Color operator*(const float s);

		operator uint32_t () const;
		operator glm::vec4() const;
		operator ImVec4 () const;
	};

}

#endif