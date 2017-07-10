#include "accord/material.h"
#include "accord/hierarchy.h"

Material::Material() {
	auto ctx = Context::get_instance();
	auto ml = ctx->get_material_library();
	ml->add(this);
}
Material::~Material() {
	auto ctx = Context::get_instance();
	auto ml = ctx->get_material_library();
	ml->remove(this);
}

bool Material::create(std::string vs, std::string fs) {
	m_program_id = _create_program(vs, fs);

	if (m_program_id == -1) return false;

	glUseProgram(m_program_id);

	return true;
}

void Material::bind_model(glm::mat4 model_mat) {
	int32_t m_location = glGetUniformLocation(m_program_id, "model");
	if (m_location == -1)    std::cout << "Model matrix not found in shader!" << std::endl;
	glUniformMatrix4fv(m_location, 1, false, &model_mat[0][0]);
}