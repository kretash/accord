#pragma once

#include "accord/accord.h"
#include "accord/input.h"

class Camera : virtual public Component {
public:

	Camera()
	{
		m_proj = glm::perspective(75.0f, 16.0f / 9.0f, 0.1f, 100.0f);
	}
	~Camera() {}

	virtual void awake() {}
	virtual void start() {}
	virtual void prepare() {}

	virtual void update() {

		Input* input = get_component<Input>();

		mouse this_mouse = input->get_mouse();
		if (this_mouse.lmb && !m_drag_camera) {
			m_last_mouse = this_mouse;
			m_drag_camera = true;
		}
		else if (this_mouse.lmb && m_drag_camera) {
			m_movedir.x -= (m_last_mouse.x - this_mouse.x) * m_mouse_speed;
			m_movedir.y -= (m_last_mouse.y - this_mouse.y) * m_mouse_speed;
			m_last_mouse = this_mouse;
		}
		else {
			m_drag_camera = false;
		}

		if (input->key_down(SDLK_UP)) {
			m_movedir.y -= m_dir_speed;
		}
		if (input->key_down(SDLK_DOWN)) {
			m_movedir.y += m_dir_speed;
		}
		if (input->key_down(SDLK_RIGHT)) {
			m_movedir.x += m_dir_speed;
		}
		if (input->key_down(SDLK_LEFT)) {
			m_movedir.x -= m_dir_speed;
		}

		m_direction.x = sinf(-m_movedir.x);
		m_direction.z = cosf(-m_movedir.x);
		m_direction.y = tanf(-m_movedir.y);

		if (input->key_down(SDLK_w)) {
			m_eye = m_eye + (m_direction*m_movement_speed);
		}
		if (input->key_down(SDLK_s)) {
			m_eye = m_eye - (m_direction*m_movement_speed);
		}
		if (input->key_down(SDLK_a)) {
			glm::vec3 strafe = glm::vec3(-m_direction.z, 0.0f, m_direction.x);
			m_eye = m_eye - (strafe*m_movement_speed);
		}
		if (input->key_down(SDLK_d)) {
			glm::vec3 strafe = glm::vec3(-m_direction.z, 0.0f, m_direction.x);
			m_eye = m_eye + (strafe*m_movement_speed);
		}
		if (input->key_down(SDLK_q)) {
			glm::vec3 high = glm::vec3(0.0f, -1.0f, 0.0f);
			m_eye = m_eye + (high*m_movement_speed);
		}
		if (input->key_down(SDLK_e)) {
			glm::vec3 high = glm::vec3(0.0f, 1.0f, 0.0f);
			m_eye = m_eye + (high*m_movement_speed);
		}

		m_view = glm::lookAt(m_eye, m_eye + m_direction, glm::vec3(0.0f, -1.0f, 0.0f));
	}

	virtual void late_update() {}
	virtual void shutdown() {}

	glm::mat4 get_view() const { return m_view; }
	glm::mat4 get_proj() const { return m_proj; }

	void render();

private:

	const float m_movement_speed = 0.1f;
	const float m_dir_speed = 0.01f;
	const float m_mouse_speed = 0.001f;

	glm::vec2 m_movedir = glm::vec2(0.54f, 0.26f);
	glm::vec3 m_eye = glm::vec3(2.5f, 1.8f, -4.2f);
	glm::vec3 m_direction = glm::vec3(0.0f, 5.0f, -10.0f);

	glm::mat4 m_view = glm::mat4(1.0f);
	glm::mat4 m_proj = glm::mat4(1.0f);

	mouse m_last_mouse;
	bool m_drag_camera = false;

};