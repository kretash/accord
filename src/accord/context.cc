#include "accord/context.h"
#include "accord/accord.h"
#include "accord/editor.h"
#include "accord/input.h"

void Context::awake() {

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

	m_hierarchy = std::shared_ptr<Hierarchy>(new Hierarchy);
	m_material_library = std::make_shared<MaterialLibrary>();
	m_input = std::make_shared<Input>();
	m_editor = std::make_shared<Editor>(m_window);

	for (Component* comp : m_components) comp->awake();

	_call_user_function("awake");

	running = true;
}

void Context::start() {
	for (Component* comp : m_components) comp->start();

	_call_user_function("start");
}

void Context::prepare() {
	for (Component* comp : m_components) comp->prepare();

	_call_user_function("prepare");
}

void Context::update() {

	m_frame_start = std::chrono::high_resolution_clock::now();

	for (Component* comp : m_components) comp->update();

	_call_user_function("update");

	running = m_input->quit() == false;
}

void Context::late_update() {

	for (Component* comp : m_components) comp->late_update();

	_call_user_function("late_update");
}

void Context::render() {
	_call_user_function("render");
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

	_call_user_function("shutdown");
	running = false;
	for (Component* comp : m_components) comp->shutdown();

	SDL_DestroyWindow(m_window);
	SDL_Quit();
}