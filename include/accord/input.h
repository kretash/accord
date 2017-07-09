#pragma once

#include <memory>
#include <map>
#include "SDL/SDL.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_sdl_gl3.h"

#include "accord/accord.h"

struct mouse {
	float x = 0.0f, y = 0.0f;
	bool lmb = false, mmb = false, rmb = false;
};

class Input : virtual public Component {
public:

	Input() {};
	~Input() {};

	virtual void awake();
	virtual void start();
	virtual void prepare();
	virtual void update();
	virtual void late_update();
	virtual void shutdown();

	bool         key_down(SDL_Keycode key);
	mouse        get_mouse();
	bool         quit();

private:
	std::map<SDL_Keycode, bool> m_downkeys;
	bool                        m_quit = false;
	mouse                       m_mouse;
};