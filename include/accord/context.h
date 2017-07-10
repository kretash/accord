#pragma once

#include <memory>
#include <iostream>
#include <cassert>
#include <chrono>

#include "accord/accord.h"
#include "accord/gl_helper.h"
#include "accord/hierarchy.h"
#include "accord/material.h"

struct render_state {
	GLuint bound_program = 0;
	GLuint bound_vao = 0;
	GLuint bound_vbuffer = 0;
	GLuint bound_ebuffer = 0;
};

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

	SDL_Window* get_window() const { return m_window; }
	std::shared_ptr<Hierarchy> get_hierarchy() const { return m_hierarchy; }
	std::shared_ptr<MaterialLibrary> get_material_library() const { return m_material_library; }

protected:
	Context() {}
	std::string m_component_name = "Context";
private:
	std::vector<Component*>                        m_components;
	std::shared_ptr<Input>                         m_input;
	std::shared_ptr<Editor>                        m_editor;
	std::shared_ptr<Hierarchy>                     m_hierarchy;
	std::shared_ptr<MaterialLibrary>               m_material_library;
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