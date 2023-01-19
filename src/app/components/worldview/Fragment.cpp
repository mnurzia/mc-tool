#include <glad/glad.h>

#include "Fragment.hpp"
#include "FragmentShaders.hpp"
#include "FragmentManager.hpp"

#include "../../../worldgen/Biome.hpp"
#include "../../../worldgen/Feature.hpp"
#include "../../../worldgen/Layer.hpp"

namespace App::Components::WorldView {

	static FragmentShaders* shaders_loaded = NULL;

	Fragment::Fragment(WorldGen::Generator* gen_, FragmentManager* parent_) {
		parent = parent_;
		gen.init(gen_->getVersion());
		gen.applySeed(gen_->getSeed());
		slimechunks = (uint32_t*)malloc(sizeof(uint32_t) * 64 * 64);
	}

	void Fragment::compress() {
		uint32_t* in = (uint32_t*)data;
		for (int32_t y = 0; y < 1024; y++) {
			for (int32_t x = 0; x < 1024; x++) {
				//uint8_t temp = uint8_t(in[y * 1024 + x]);
				//data[y * 1024 + x] = temp;
				in[y * 1024 + x] = parent->palette_colors[in[(y * 1024 + x)] % 256];
			}
		}
		//data = (uint8_t*)realloc(data, sizeof(uint8_t) * 1024 * 1024);
	}

	void Fragment::genSlimeChunks() {
		int64_t seed = gen.getSeed();
		int32_t fx = getX() >> 4;
		int32_t fz = getZ() >> 4;
		for (int32_t y = 0; y < 64; y++) {
			for (int32_t x = 0; x < 64; x++) {
				int32_t cx = fx + x;
				int32_t cz = fz + y;
				int64_t schk = seed +
					(int64_t)(cx * cx * 0x4c1906) +
					(int64_t)(cx * 0x5ac0db) +
					(int64_t)(cz * cz) * 0x4307a7L +
					(int64_t)(cz * 0x5f24f) ^ 0x3ad8025f;
				WorldGen::JavaRandom::setSeed(&schk);
				if (WorldGen::JavaRandom::nextInt(&schk, 10) == 0) {
					slimechunks[y * 64 + x] = 0xFFFFFFFF;
				} else {
					slimechunks[y * 64 + x] = 0;
				}
			}
		}
	}

	void Fragment::generate() {
		if (data) {
			free(data);
		}
		WorldGen::Layer* top = gen.getLayer(WorldGen::Generator::ID::VORONOI_ZOOM_1);
		data = (uint8_t*)top->allocCache(1024, 1024);
		top->genArea((int32_t*)data, x, z, 1024, 1024);
		gen.getFeatures(features, x, z, 1024, 1024, (int32_t*)data);
		compress();
		genSlimeChunks();
		state = Fragment::State::GENERATED;
	}

	uint8_t* Fragment::getData() {
		return data;
	}

	void Fragment::reset() {
		features.clear();
		opacity = 0.0f;
		if (out_texture) {
			unbindTexture();
		}
		out_texture = 0;
		viewport = NULL;
	}

	void Fragment::destroy() {
		reset();
		if (data) {
			free(data);
		}
		if (slimechunks) {
			free(slimechunks);
		}
		gen.destroy();
	}

	void Fragment::setX(int32_t x_) {
		x = x_;
		state = Fragment::State::UNINITIALIZED;
		reset();
	}

	int32_t Fragment::getX() {
		return x;
	}

	void Fragment::setZ(int32_t z_) {
		z = z_;
		state = Fragment::State::UNINITIALIZED;
		reset();
	}

	int32_t Fragment::getZ() {
		return z;
	}

	void Fragment::setOpacity(float opacity_) {
		opacity = opacity_;
	}

	float Fragment::getOpacity() {
		return opacity;
	}

	void Fragment::setAccess(int64_t time) {
		last_accessed = time;
	}

	int64_t Fragment::getAccess() {
		return last_accessed;
	}

	void Fragment::setState(Fragment::State state_) {
		state = state_;
	}

	Fragment::State Fragment::getState() {
		return state;
	}

	std::vector<WorldGen::Feature>& 
		Fragment::getFeatures() {
		return features;
	}

	void Fragment::bindTexture() {
		if (shaders_loaded == NULL) {
			shaders_loaded = new FragmentShaders();
		}
		out_texture = 0;
		glGenTextures(1, &out_texture);
		glBindTexture(GL_TEXTURE_2D, out_texture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1024, 1024, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		schk_texture = 0;
		glGenTextures(1, &schk_texture);
		glBindTexture(GL_TEXTURE_2D, schk_texture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 64, 64, 0, GL_RGBA, GL_UNSIGNED_BYTE, slimechunks);
	}

	void Fragment::unbindTexture() {
		glDeleteTextures(1, &out_texture);
		glDeleteTextures(1, &schk_texture);
		out_texture = 0;
		schk_texture = 0;
	}

	ImTextureID Fragment::getSlimeTexture() {
		return (void*)(int64_t)schk_texture;
	}

	void Fragment::render(const ImDrawList* parent_list, const ImDrawCmd* cmd) {
		Fragment* cur = (Fragment*)cmd->UserCallbackData;
		ImDrawData* draw_data = cur->viewport->DrawData;
		float L = draw_data->DisplayPos.x;
		float R = draw_data->DisplayPos.x + draw_data->DisplaySize.x;
		float T = draw_data->DisplayPos.y;
		float B = draw_data->DisplayPos.y + draw_data->DisplaySize.y;
		const float ortho_projection[4][4] = {
			{ 2.0f/(R-L),   0.0f,         0.0f,   0.0f },
			{ 0.0f,         2.0f/(T-B),   0.0f,   0.0f },
			{ 0.0f,         0.0f,        -1.0f,   0.0f },
			{ (R+L)/(L-R),  (T+B)/(B-T),  0.0f,   1.0f },
		};
		GLint last_program = 0;
		GLint last_texture = 0;
		GLint last_active = 0;
		//glGetIntegerv(GL_CURRENT_PROGRAM, &last_program);
		glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture);
		glGetIntegerv(GL_ACTIVE_TEXTURE, &last_active);
		//glUseProgram(shaders_loaded->getShaderID());
		glUniformMatrix4fv(glGetUniformLocation(shaders_loaded->getShaderID(), "proj_mtx"), 1, GL_FALSE, &ortho_projection[0][0]);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, cur->out_texture);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_1D, cur->parent->getPalette());
		glDrawElementsBaseVertex(GL_TRIANGLES, (GLsizei)cmd->ElemCount, sizeof(ImDrawIdx) == 2 ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT, (void*)(intptr_t)(cmd->IdxOffset * sizeof(ImDrawIdx)), (GLint)cmd->VtxOffset);
		//glUseProgram(last_program);
		glActiveTexture(last_active);
		glBindTexture(GL_TEXTURE_2D, last_texture);
	}

	void Fragment::setViewport(ImGuiViewport* viewport_) {
		viewport = viewport_;
	}

	WorldGen::Generator& Fragment::getGenerator() {
		return gen;
	}

	FragmentManager* Fragment::getParent() {
		return parent;
	}

}
