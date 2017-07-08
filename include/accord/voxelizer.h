#pragma once

#include <stdint.h>
#include <memory>

#include "gl_helper.h"

class Voxelizer : public drawable{
public:
	Voxelizer()  {}
	~Voxelizer() {
		for (size_t i = 0; i < m_cubes.size(); ++i) {
			delete m_cubes[i];
			m_cubes[i] = nullptr;
		}
		m_cubes.clear();
		m_cubes.shrink_to_fit();
	}

	void generate(std::string other);
	void draw( GLint program );

	void hide() {
		for (auto cube : m_cubes) cube->active = false;
	}

	void show() {

		size_t counter = 0;

		for (int32_t i = 0; i < m_buffer.size(); ++i) {
			for (int32_t e = 0; e < m_buffer[i].size(); ++e) {
				for (int32_t o = 0; o < m_buffer[i][e].size(); ++o) {

					uint8_t* c = &m_buffer[i][e][o];
					auto cube = m_cubes[counter++];

					cube->active = *c != 0;
					cube->model = glm::translate(glm::mat4(1.0f), glm::vec3(i, e, o));
				}
			}
		}
	}

private:
	std::vector<glm::vec3> m_clone;
	glm::vec3 m_start;
	glm::vec3 m_end;
	float m_step;
	const int32_t size = 8;
	std::vector<std::vector<std::vector<uint8_t>>> m_buffer;
	std::vector<drawable*> m_cubes;
};