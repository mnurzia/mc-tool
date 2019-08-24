#include "App.hpp"

#include <cstdio>
#include <cstdlib>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <imgui/imgui.h>
#include <imgui/examples/imgui_impl_glfw.h>
#include <imgui/examples/imgui_impl_opengl3.h>
#include <imgui/imgui_freetype.h>
#include <imgui/imgui_freetype.cpp>

#include <spdlog/sinks/stdout_color_sinks.h>

#include <stb_image.h>

#include <pastel_icons.h>

#include "ImGuiEx.hpp"
#include "components/worldview/WorldView.hpp"
#include "components/threadstatus/ThreadStatus.hpp"
#include "components/menubar/MenuBar.hpp"
#include "components/nbtview/NBTView.hpp"
#include "components/fontmanager/FontManager.hpp"
#include "components/functioneditor/FunctionEditor.hpp"
#include "components/logview/LogView.hpp"
#include "components/logview/Sink.hpp"

#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

static void glfw_error_callback(int error, const char* description) {
	fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

namespace App {

	void App::initGLFW() {
		// Setup window
		glfwSetErrorCallback(glfw_error_callback);
		if (!glfwInit()) {
			printf("Couldn't initialize GLFW context!\n");
			exit(1);
		}
		
		// Decide GL+GLSL versions
#if __APPLE__
		// GL 3.2 + GLSL 150
		glsl_version = "#version 150";
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
#else
		// GL 4.5 + GLSL 450
		glsl_version = "#version 450";
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
		//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
		//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif
		// Create window with graphics context
		window = glfwCreateWindow(1280, 720, "very cool biome explorer", NULL, NULL);
		if (window == NULL) {
			logger->critical("Error initializing GLFW: Couldn't initialize GLFW window.");
			exit(1);
		}
		glfwMakeContextCurrent(window);
		gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		glfwSwapInterval(1); // Enable vsync
		// Initialize OpenGL loader
		bool err = gladLoadGL() == 0;
		if (err) {
			logger->critical("Error initializing GLFW: Couldn't initialize GLAD.");
			exit(1);
		}
		logger->info("Initialized GLFW.");
		logger->info("OpenGL Version: {}", glGetString(GL_VERSION));
	}

	void App::initImGui() {
		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.IniFilename = NULL;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
		//io.ConfigViewportsNoAutoMerge = true;
		//io.ConfigViewportsNoTaskBarIcon = true;
		// Setup Dear ImGui style
		ImGui::StyleColorsDark();
		//ImGui::StyleColorsClassic();
		ImGuiStyle& style = ImGui::GetStyle();
		style.TabRounding = 0.0f;
		style.FrameBorderSize = 1.0f;
		style.ScrollbarRounding = 0.0f;
		style.ScrollbarSize = 10.0f;
		ImVec4* colors = ImGui::GetStyle().Colors;
		colors[ImGuiCol_Text] = ImVec4(0.95f, 0.95f, 0.95f, 1.00f);
		colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
		colors[ImGuiCol_WindowBg] = ImVec4(0.12f, 0.12f, 0.12f, 1.00f);
		colors[ImGuiCol_ChildBg] = ImVec4(0.04f, 0.04f, 0.04f, 0.50f);
		colors[ImGuiCol_PopupBg] = ImVec4(0.12f, 0.12f, 0.12f, 0.94f);
		colors[ImGuiCol_Border] = ImVec4(0.25f, 0.25f, 0.27f, 0.50f);
		colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		colors[ImGuiCol_FrameBg] = ImVec4(0.20f, 0.20f, 0.22f, 0.50f);
		colors[ImGuiCol_FrameBgHovered] = ImVec4(0.25f, 0.25f, 0.27f, 0.75f);
		colors[ImGuiCol_FrameBgActive] = ImVec4(0.30f, 0.30f, 0.33f, 1.00f);
		colors[ImGuiCol_TitleBg] = ImVec4(0.04f, 0.04f, 0.04f, 1.00f);
		colors[ImGuiCol_TitleBgActive] = ImVec4(0.04f, 0.04f, 0.04f, 1.00f);
		colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.04f, 0.04f, 0.04f, 0.75f);
		colors[ImGuiCol_MenuBarBg] = ImVec4(0.18f, 0.18f, 0.19f, 1.00f);
		colors[ImGuiCol_ScrollbarBg] = ImVec4(0.24f, 0.24f, 0.26f, 0.75f);
		colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.41f, 0.41f, 0.41f, 0.75f);
		colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.62f, 0.62f, 0.62f, 0.75f);
		colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.94f, 0.92f, 0.94f, 0.75f);
		colors[ImGuiCol_CheckMark] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
		colors[ImGuiCol_SliderGrab] = ImVec4(0.41f, 0.41f, 0.41f, 0.75f);
		colors[ImGuiCol_SliderGrabActive] = ImVec4(0.62f, 0.62f, 0.62f, 0.75f);
		colors[ImGuiCol_Button] = ImVec4(0.20f, 0.20f, 0.22f, 1.00f);
		colors[ImGuiCol_ButtonHovered] = ImVec4(0.25f, 0.25f, 0.27f, 1.00f);
		colors[ImGuiCol_ButtonActive] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
		colors[ImGuiCol_Header] = ImVec4(0.18f, 0.18f, 0.19f, 1.00f);
		colors[ImGuiCol_HeaderHovered] = ImVec4(0.25f, 0.25f, 0.27f, 1.00f);
		colors[ImGuiCol_HeaderActive] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
		colors[ImGuiCol_Separator] = ImVec4(0.25f, 0.25f, 0.27f, 1.00f);
		colors[ImGuiCol_SeparatorHovered] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
		colors[ImGuiCol_SeparatorActive] = ImVec4(0.62f, 0.62f, 0.62f, 1.00f);
		colors[ImGuiCol_ResizeGrip] = ImVec4(0.30f, 0.30f, 0.33f, 0.75f);
		colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.41f, 0.41f, 0.41f, 0.75f);
		colors[ImGuiCol_ResizeGripActive] = ImVec4(0.62f, 0.62f, 0.62f, 0.75f);
		colors[ImGuiCol_Tab] = ImVec4(0.21f, 0.21f, 0.22f, 1.00f);
		colors[ImGuiCol_TabHovered] = ImVec4(0.37f, 0.37f, 0.39f, 1.00f);
		colors[ImGuiCol_TabActive] = ImVec4(0.30f, 0.30f, 0.33f, 1.00f);
		colors[ImGuiCol_TabUnfocused] = ImVec4(0.12f, 0.12f, 0.12f, 0.97f);
		colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.18f, 0.18f, 0.19f, 1.00f);
		colors[ImGuiCol_DockingPreview] = ImVec4(0.26f, 0.59f, 0.98f, 0.50f);
		colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
		colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
		colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
		colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
		colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
		colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.50f);
		colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
		colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
		colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
		colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
		colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
		style.WindowMenuButtonPosition = ImGuiDir_Right;

		// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		// Setup Platform/Renderer bindings
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init(glsl_version);
		logger->info("Initialized ImGui.");
	}

	void App::init() {
		logger_sinks.push_back(std::make_shared<spdlog::sinks::stderr_color_sink_mt>());
		logger_sinks.push_back(std::make_shared<Components::LogView::Sink>());
		logger = std::make_shared<spdlog::logger>("app", begin(logger_sinks), end(logger_sinks));
		logger->set_level(spdlog::level::debug);
		logger_sinks[0]->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%^%l%$] [%n] %v");
		logger_sinks[0]->set_level(spdlog::level::debug);
		logger_sinks[1]->set_pattern("%Y-%m-%d %H:%M:%S.%e");
		logger_sinks[1]->set_level(spdlog::level::debug);
		initGLFW();
		initImGui();
		font_manager = new Components::FontManager::FontManager(this);
		log_viewer = new Components::LogView::LogView(this);
		addComponent(std::unique_ptr<UIComponent>(new Components::ThreadStatus::ThreadStatus(this)));
		addComponent(std::unique_ptr<UIComponent>(new Components::MenuBar::MenuBar(this)));
		addComponent(std::unique_ptr<UIComponent>(new Components::NBTView::NBTView(this, "assets/map_5.dat")));
		addComponent(std::unique_ptr<UIComponent>(new Components::FunctionEditor::FunctionEditor(this)));
	}

	void App::run() {
		while (!glfwWindowShouldClose(window)) {
			static bool show_demo_window = true;
			glfwPollEvents();

			ImGuiIO& io = ImGui::GetIO();

			// Start the Dear ImGui frame
			if (font_manager->updateRebuild()) {
				ImGui_ImplOpenGL3_DestroyDeviceObjects();
				ImGui_ImplOpenGL3_CreateDeviceObjects();
			}
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			ImGui::ShowDemoWindow(&show_demo_window);

			active_components.erase(
				std::remove_if(
					active_components.begin(),
					active_components.end(),
					[](auto& in) {
						return in.get()->canQuit();
					}
				),
				active_components.end()
			);
			font_manager->render();
			log_viewer->render();

			for (int i = 0; i < active_components.size(); i++) {
				UIComponent* c = active_components[i].get();
				Util::UUID id = c->getID();
				ImGui::PushID(c);
				c->render();
				ImGui::PopID();
			}

			ImGui::Render();

			int display_w, display_h;
			glfwGetFramebufferSize(window, &display_w, &display_h);
			glViewport(0, 0, display_w, display_h);
			glClearColor(0.5, 0.5, 0.5, 1.0);
			glClear(GL_COLOR_BUFFER_BIT);
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
				GLFWwindow* backup_current_context = glfwGetCurrentContext();
				ImGui::UpdatePlatformWindows();
				ImGui::RenderPlatformWindowsDefault();
				glfwMakeContextCurrent(backup_current_context);
			}

			glfwSwapBuffers(window);
		}
	}

	void App::destroy() {
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();

		glfwDestroyWindow(window);
		glfwTerminate();
		delete font_manager;
	}

	DispatchQueue::DispatchQueue& App::getDispatchQueue() {
		return task_queue;
	}

	ImTextureID App::bindTexture(ImTextureID in, int32_t* data, int32_t w, int32_t h) {
		if (in == 0) {
			GLuint out_text;
			glGenTextures(1, &out_text);
			glBindTexture(GL_TEXTURE_2D, out_text);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
			return (void*)(intptr_t)out_text;
		} else {
			glBindTexture(GL_TEXTURE_2D, (GLuint)(intptr_t)in);
			glTexSubImage2D(GL_TEXTURE_2D, 0, 0, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
			return in;
		}
	}

	void App::unbindTexture(ImTextureID in) {
		GLuint val = (GLuint)(intptr_t)in;
		glDeleteTextures(1, &val);
	}

	void App::addComponent(std::unique_ptr<UIComponent>&& in) {
		logger->debug("New window: " + in->getID().as_string());
		active_components.push_back(std::move(in));
	}

	Components::FontManager::FontManager* App::getFontManager() {
		return font_manager;
	}

	Components::LogView::LogView* App::getLogView() {
		return log_viewer;
	}

	std::shared_ptr<spdlog::logger> App::getLogger(std::string name) {
		return std::make_shared<spdlog::logger>(name, begin(logger_sinks), end(logger_sinks));
	}

	std::shared_ptr<spdlog::logger> App::getLogger() {
		return logger;
	}

}