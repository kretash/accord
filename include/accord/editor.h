#pragma once

#include "accord/accord.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_sdl_gl3.h"

#include <chrono>
#include <string>

class Editor : Component {
public:
	~Editor() {}

	virtual void awake() {}
	virtual void start() {}
	virtual void prepare() {}
	virtual void update() {}

	virtual void render() {

		bool show_fps = true;
		ImGui_ImplSdlGL3_NewFrame(m_window);

		if (show_fps)
		{
			ImGui::Begin("Voxels", &show_fps);
			ImGui::Text(std::string("FPS:" + std::to_string((int32_t)55)).c_str());
			
			ImGui::End();
		}

		ImGui::Render();
	}

	virtual void shutdown() {
		ImGui_ImplSdlGL3_Shutdown();
	}

protected:

private:
	Editor(SDL_Window* window) {
		ImGui_ImplSdlGL3_Init(window);
	}

	SDL_Window* m_window
};