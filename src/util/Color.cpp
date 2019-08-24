#include "Color.hpp"

namespace Util {

	Color::Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
		this->r = r;
		this->g = g;
		this->b = b;
		this->a = a;
	}

	Color::Color(uint8_t l, uint8_t a) {
		this->r = l;
		this->g = l;
		this->b = l;
		this->a = a;
	}

	Color::Color(uint32_t rgba) {
		r = (uint8_t)(rgba & 0xFF);
		g = (uint8_t)((rgba >> 8) & 0xFF);
		b = (uint8_t)((rgba >> 16) & 0xFF);
		a = (uint8_t)((rgba >> 24) & 0xFF);
	}

	Color::Color(uint8_t* rgb, uint8_t a) {
		this->r = rgb[0];
		this->g = rgb[1];
		this->b = rgb[2];
		this->a = a;
	}

	Color Color::operator+(const Color& c) {
		Color out;
		float sa = float(a) / 255.0f;
		float sr = float(r) / 255.0f;
		float sg = float(g) / 255.0f;
		float sb = float(b) / 255.0f;
		float da = float(c.a) / 255.0f;
		float dr = float(c.r) / 255.0f;
		float dg = float(c.g) / 255.0f;
		float db = float(c.b) / 255.0f;
		float oa = sa + da * (1 - sa);
		out.a = uint8_t(oa * 255.0f);
		out.r = uint8_t((sr * sa + dr * da * (1 - sa)) / oa * 255.0f);
		out.g = uint8_t((sg * sa + dg * da * (1 - sa)) / oa * 255.0f);
		out.b = uint8_t((sb * sa + db * da * (1 - sa)) / oa * 255.0f);
		return out;
	}

	Color Color::operator*(const float s) {
		Color out;
		out.r = (uint8_t)(float(r) * s);
		out.g = (uint8_t)(float(g) * s);
		out.b = (uint8_t)(float(b) * s);
		out.a = a;
		return out;
	}

	Color::operator uint32_t() const {
		uint32_t out = 0;
		out |= a;
		out <<= 8;
		out |= b;
		out <<= 8;
		out |= g;
		out <<= 8;
		out |= r;
		return out;
	}

	Color::operator glm::vec4() const {
		glm::vec4 out;
		out.r = float(r) / 255.0f;
		out.g = float(g) / 255.0f;
		out.b = float(b) / 255.0f;
		out.a = float(a) / 255.0f;
		return out;
	}

	Color::operator ImVec4() const {
		return ImVec4(this->operator glm::vec4());
	}

}