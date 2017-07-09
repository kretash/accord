#pragma once

#include <stdio.h>
#include <memory>
#include <math.h>
#include <vector>
#include <iostream>
#include <algorithm>

#define GLEW_STATIC
#include "GL/glew.h"
#include <gl/GL.h>

#include "SDL/SDL.h"
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.inl>

#define distance(a,b) std::max( abs(a-b), abs(b-a) )

class Component;

template<typename T>
T* get_component();

template<typename T>
std::vector<T*> get_components();

void add_component_to_context(Component* c);

class Component {
public:
	Component() {
		add_component_to_context(this);
	}
	~Component() {}

	virtual void awake() = 0;
	virtual void start() = 0;
	virtual void prepare() = 0;
	virtual void update() = 0;
	virtual void late_update() = 0;
	virtual void shutdown() = 0;

protected:
	std::string m_component_name = "Component";
private:
};

enum RenderLayer {
	l_Default = 0,
	l_None = 999
};

struct drawable {

	bool active = true;
	RenderLayer layer = RenderLayer::l_Default;

	GLuint v_buffer;
	std::vector<glm::vec3> v_data;

	GLuint e_buffer;
	GLuint e_count;
	std::vector<uint32_t> e_data;

	GLint m_location;
	GLuint vao;
	glm::mat4 model;

	void bind_model(GLint program) {

		m_location = glGetUniformLocation(program, "model");
		if (m_location == -1)    std::cout << "Model matrix not found in shader!" << std::endl;
		glUniformMatrix4fv(m_location, 1, false, &model[0][0]);

	}
};