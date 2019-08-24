#ifndef APP_APP_HPP
#define APP_APP_HPP

#include <memory>
#include <vector>

#include <imgui/imgui.h>

#include <spdlog/spdlog.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "dispatchqueue/DispatchQueue.hpp"
#include "components/UIComponent.hpp"

namespace App::Components {
	namespace LogView {
		class LogView;
		class Sink;
	}

	namespace FontManager {
		class FontManager;
	}
}

namespace App {

	class App {

		/*
		 * @brief GLSL version string for internal UI use
		 */
		const char* glsl_version = "#version 130";

		/*
		 * @brief GLFW window object
		 */
		GLFWwindow* window = NULL;

		/*
		 * @brief Queue used for async task loading
		 */
		DispatchQueue::DispatchQueue task_queue;

		/*
		 * @brief Font/icon manager.
		 */
		Components::FontManager::FontManager* font_manager;

		/*
		 * @brief Log viewer.
		 */
		Components::LogView::LogView* log_viewer;

		/**
		 * @brief List of components (windows)
		 */
		std::vector<std::unique_ptr<UIComponent>> active_components;

		/**
		 * @brief Default global logger
		 */
		std::shared_ptr<spdlog::logger> logger;

		/**
		 * @brief Default global logger's sinks
		 */
		std::vector<spdlog::sink_ptr> logger_sinks;

		/*
		 * @brief Initialize OpenGL and GLFW through GL3W and GLFW, respectively
		 */
		void initGLFW();

		/*
		 * @brief Initialize ImGui.
		 */
		void initImGui();

	public:
		/*
		 * @brief Initialize the app, calls initGLFW() and initImGui();
		 */
		void init();

		/*
		 * @brief App main loop
		 */
		void run();

		/*
		 * @brief Cleanup
		 */
		void destroy();

		/**
		 * @brief Get the dispatch queue
		 */
		DispatchQueue::DispatchQueue& getDispatchQueue();

		/**
		 * @brief Get a texture ID by uploading @c data to the GPU.
		 */
		ImTextureID bindTexture(ImTextureID in, int32_t* data, int32_t w, int32_t h);

		/**
		 * @brief Unbind a texture ID.
		 */
		void unbindTexture(ImTextureID in);

		/**
		 * @brief Add a component to the local list.
		 */
		void addComponent(std::unique_ptr<UIComponent>&& in);

		/**
		 * @brief Get a pointer to a (new) logger
		 */
		std::shared_ptr<spdlog::logger> getLogger(std::string name);

		/**
		 * @brief Get a pointer to the logger
		 */
		std::shared_ptr<spdlog::logger> getLogger();

		/**
		 * @brief Get the font manager.
		 */
		Components::FontManager::FontManager* getFontManager();

		/**
		 * @brief Get the log viewer.
		 */
		Components::LogView::LogView* getLogView();
	};

}

#endif