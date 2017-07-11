#pragma once

#include <chrono>
#include <string>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_sdl_gl3.h"

#include "accord/accord.h"
#include "accord/context.h"

class Editor : public Component {
public:
	Editor(SDL_Window* window);
	~Editor();

	virtual void awake();
	virtual void start();
	virtual void prepare();
	virtual void update();
	virtual void late_update();
	virtual void shutdown();

	void render();

protected:

private:
	SDL_Window*              m_window = nullptr;
	std::shared_ptr<Context> m_context;
  bool                     m_w_hierarchy = false;
  bool                     m_w_inspector = false;
};