#include "accord/context.h"
#include "accord/accord.h"
#include "accord/editor.h"
#include "accord/input.h"

void Context::awake() {

	m_input = std::make_shared<Input>();
	m_editor = std::make_shared<Editor>(m_window);

	for (Component* comp : m_components) comp->awake();

	running = true;
}

void Context::start() {
	for (Component* comp : m_components) comp->start();
}

void Context::prepare() {
	for (Component* comp : m_components) comp->prepare();
}

void Context::update() {

	m_frame_start = std::chrono::high_resolution_clock::now();

	for (Component* comp : m_components) comp->update();

	running = m_input->quit() == false;
}

void Context::late_update() {

	for (Component* comp : m_components) comp->late_update();
}

void Context::swap() {

	m_editor->render();

	SDL_GL_SwapWindow(m_window);

	std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
	std::chrono::duration<float> time_span = std::chrono::duration_cast<std::chrono::duration<float>>(t2 - m_frame_start);
	m_frame_time = time_span.count();
	m_time += time_span.count();
	m_frame_count += 1;

	if (distance(m_last_fps_time, m_time) > 0.5f) {
		m_last_fps_time = m_time;
		float this_frame_fps = 1.0f / m_frame_time;
		float past_fps = m_cumulative_fps;

		if (distance(m_cumulative_fps, this_frame_fps) > 5.0f) {
			m_cumulative_fps = this_frame_fps;
		}
		else {
			m_cumulative_fps = m_cumulative_fps*0.9f + this_frame_fps*0.1f;
		}

		if (distance(m_cumulative_fps, past_fps) < 3.0f)m_cumulative_fps = past_fps;
	}
}

void Context::shutdown() {
	running = false;
	for (Component* comp : m_components) comp->shutdown();

	SDL_DestroyWindow(m_window);
	SDL_Quit();
}