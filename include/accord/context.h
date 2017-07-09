#pragma once

#include <memory>
#include <iostream>
#include <cassert>
#include <chrono>

#include "accord/accord.h"
#include "accord/gl_helper.h"

struct render_state {
	GLuint bound_program = 0;
	GLuint bound_vao = 0;
	GLuint bound_vbuffer = 0;
	GLuint bound_ebuffer = 0;
};

class Editor;
class Input;

class Context {
public:
	static std::shared_ptr<Context> get_instance() {
		static std::shared_ptr<Context> m_instance(new Context);
		return m_instance;
	}

	void add_component(Component* c) {
		m_components.push_back(c);
	}

	~Context() {}

	template <typename T>
	T* get_component() {
		for (Component* comp : m_components) {
			T* p = dynamic_cast< T* >(comp);
			if (p != nullptr) return p;
		}
		return nullptr;
	}

	template <typename T>
	T* get_components() {
		std::vector<T*> v;
		for (component* comp : m_components) {
			T* p = dynamic_cast< T* >(comp);
			if (p != nullptr) v.push_back(p);
		}
		return v;
	}

	void awake();
	void start();
	void prepare();
	void update();
	void late_update();

	void swap();

	void shutdown();

	bool running = false;
	render_state m_render_state = {};

	SDL_Window* get_window() { return m_window; }

protected:
	Context() {
		
		int sdl_init_ = SDL_Init(SDL_INIT_VIDEO);
		if (sdl_init_ < 0)
			std::cout << "SDL error ->" << SDL_GetError() << std::endl;

		m_window = SDL_CreateWindow("Accord", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
			1280, 720, SDL_WINDOW_OPENGL);

		m_gl_context = SDL_GL_CreateContext(m_window);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);
		SDL_GL_SetSwapInterval(1);

		glewExperimental = GL_TRUE;
		GLenum glew_init_;
		if ((glew_init_ = glewInit()) != GLEW_OK) {
			std::cout << glewGetErrorString(glew_init_) << std::endl;
			assert(false && "GLEW INIT FAILED");
		}
		
#if DEBUG
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);

		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(opengl_error_callback, nullptr);
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, true);
#endif
	}
	std::string m_component_name = "Context";
private:
	std::vector<Component*>                        m_components;
	std::shared_ptr<Input>                         m_input;
	std::shared_ptr<Editor>                        m_editor;
	SDL_Window*                                    m_window = nullptr;
	SDL_GLContext                                  m_gl_context = {};

	std::chrono::high_resolution_clock::time_point m_frame_start;

public:
	float                                          m_time = 0.0f;
	float                                          m_last_fps_time = -1.0f;
	uint64_t                                       m_frame_count = 0;
	float                                          m_frame_time = 0.016f;
	float                                          m_cumulative_fps = 60.0f;
};


#ifdef TEMPLATE_FUNCTIONS

template<typename T>
T* get_component() {
	return Context::get_instance()->get_component<T>();
}

template<typename T>
std::vector<T*> get_components() {
	return Context::get_instance()->get_components<T>();
}

void add_component_to_context(Component* c) {

	auto ctx = Context::get_instance();
	ctx->add_component(c);
}

#endif