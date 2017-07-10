#include "accord/input.h"

void Input::awake() {}
void Input::start() {}
void Input::prepare() {}

void Input::update() {
	SDL_Event event_;
	while (SDL_PollEvent(&event_)) {
		ImGui_ImplSdlGL3_ProcessEvent(&event_);
		switch (event_.type) {
		case SDL_QUIT:
			m_quit = true;
			break;
		case SDL_KEYDOWN:
			if (m_downkeys.find(event_.key.keysym.sym) == m_downkeys.end())
				m_downkeys.insert(std::make_pair(event_.key.keysym.sym, true));
			else
				m_downkeys[event_.key.keysym.sym] = true;
			break;
		case SDL_KEYUP:
			m_downkeys[event_.key.keysym.sym] = false;
			break;
		case SDL_MOUSEBUTTONDOWN:
			m_mouse.lmb |= event_.button.button == SDL_BUTTON_LEFT;
			m_mouse.mmb |= event_.button.button == SDL_BUTTON_MIDDLE;
			m_mouse.rmb |= event_.button.button == SDL_BUTTON_RIGHT;
			break;
		case SDL_MOUSEBUTTONUP:
			m_mouse.lmb &= !(event_.button.button == SDL_BUTTON_LEFT);
			m_mouse.mmb &= !(event_.button.button == SDL_BUTTON_MIDDLE);
			m_mouse.rmb &= !(event_.button.button == SDL_BUTTON_RIGHT);
			break;
		case SDL_MOUSEMOTION:
			m_mouse.x = (float)event_.button.x;
			m_mouse.y = (float)event_.button.y;
		default:
			break;
		}
	}
}

void Input::late_update() {};
void Input::shutdown() {};

bool Input::key_down(SDL_Keycode key) {
	auto found = m_downkeys.find(key);
	if (found == m_downkeys.end()) return false;
	return m_downkeys[key];
}

mouse Input::get_mouse() const { return m_mouse; }
bool Input::quit() const { return m_quit; }