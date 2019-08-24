#ifndef APP_COMPONENTS_WORLDVIEW_FRAGMENT_HPP
#define APP_COMPONENTS_WORLDVIEW_FRAGMENT_HPP

#include <cstdint>

#include <memory>
#include <vector>

#include <glad/glad.h>
#include <imgui/imgui.h>

#include "../../../worldgen/Generator.hpp"
#include "../../../worldgen/Feature.hpp"

namespace App::Components::WorldView {

	class FragmentManager;

	class Fragment {

	public:
		enum class State {
			UNINITIALIZED,
			PROCESSING,
			GENERATED,
			BOUND
		};

	private:

		/**
		 * @brief The parent FragmentManager.
		 */
		FragmentManager* parent;

		/*
		 * @brief Stores biome data about a certain area.
		 */
		uint8_t* data = NULL;

		/**
		 * @brief Stores slime chunks.
		 */
		uint32_t* slimechunks = NULL;

		/**
		 * @brief Stores feature data about the same area.
		 */
		std::vector<WorldGen::Feature> features;

		/*
		 * @brief X location for the area.
		 */
		int32_t x = 0;

		/*
		 * @brief Z location for the area.
		 */
		int32_t z = 0;

		/**
		 * @brief @c true if the data has been generated, false if not.
		 */
		State state = State::UNINITIALIZED;

		/*
		 * @brief The generator used for this area.
		 */
		WorldGen::Generator gen;

		/**
		 * @brief The last render frame idx this was accessed.
		 *
		 * Used for cache eviction.
		 */
		int64_t last_accessed = 0;

		/**
		 * @brief Used to enumerate this fragment's texture ID.
		 */
		GLuint out_texture = 0;

		/**
		 * @brief Texture ID of the slime chunk overlay
		 */
		GLuint schk_texture = 0;

		/**
		 * @brief Store this fragment's ImGui viewport.
		 */
		ImGuiViewport* viewport = NULL;

		/**
		 * @brief Used to "fade in" the fragment.
		 */
		float opacity = 0.0f;

		/**
		 * @brief Deallocate certain internal state in order to
		 *		  prevent memory leakage (namely Structures).
		 */
		void reset();

	public:
		/**
		 * @brief Initialize the fragment.
		 *
		 * @param gen_ Generator to be used
		 * @param parent_ Parent FragmentManager
		 */
		Fragment(WorldGen::Generator* gen_, FragmentManager* parent_);

		/**
		 * @brief Compress biome data from int to char.
		 *
		 * Biome data is given as int32_t by the generator, however to describe
		 * every biome we only need 8 bits. Converts int32_t to int8_t.
		 * This allows for more aggressive optimization in terms of sending
		 * data to the GPU. Instead of sending 32 bits, 24 of which are empty,
		 * 8 bits are sent and used (GL_RED is used instead of GL_RGBA).
		 * I'm quite proud of this.
		 */
		void compress();

		/**
		 * @brief Generate slime chunks in the area.
		 */
		void genSlimeChunks();

		/**
		 * @brief Get slime chunk texture (64x64 RGBA)
		 */
		ImTextureID getSlimeTexture();

		/**
		 * @brief Generate the area (modifies internal state)
		 */
		void generate();

		/**
		 * @brief Deallocate all internal structures.
		 */
		void destroy();

		/**
		 * @brief Get biome data.
		 *
		 * @return Biome data as an array of int32_t colors.
		 */
		uint8_t* getData();

		/**
		 * @brief Set the area's x position.
		 */
		void setX(int32_t x_);

		/**
		 * @brief Get the area's x position.
		 */
		int32_t getX();

		/**
		 * @brief Set the area's z position.
		 */
		void setZ(int32_t z_);

		/**
		 * @brief Get the area's z position.
		 */
		int32_t getZ();

		/**
		 * @brief Set opacity.
		 */
		void setOpacity(float opacity_);

		/**
		 * @brief Get opacity.
		 */
		float getOpacity();

		/**
		 * @brief Set the last access time of this fragment.
		 *
		 * @param time The amount of frames to set.
		 */
		void setAccess(int64_t time);

		/**
		 * @brief Get the last access time of this fragment.
		 *
		 * @return The amount of frames (time) when this was last accessed.
		 */
		int64_t getAccess();

		/**
		 * @brief Set generation state.
		 */
		void setState(State state_);

		/**
		 * @brief Return if the fragment has been generated or not.
		 */
		State getState();

		/**
		 * @brief Get a list of structures.
		 */
		std::vector<WorldGen::Feature>& getFeatures();

		/**
		 * @brief Color in fragment data and send to GPU.
		 */
		void bindTexture();

		/**
		 * @brief Free GPU memory.
		 */
		void unbindTexture();

		/**
		 * @brief Render callback.
		 */
		static void render(const ImDrawList* parent_list, const ImDrawCmd* cmd);

		/**
		 * @brief Set ImGui viewport.
		 *
		 * This is needed because the renderer needs an orthographic projection matrix
		 * which varies between viewports. :(
		 */
		void setViewport(ImGuiViewport* viewport);

		/**
		 * @brief Get parent generator (used for status in the queue)
		 */
		WorldGen::Generator& getGenerator();

		/**
		 * @brief Get the parent FragmentManager
		 */
		FragmentManager* getParent();
	};

}

#endif