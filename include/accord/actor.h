#pragma once

#include "accord/accord.h"
#include "accord/hierarchy.h"

class Actor;

class Transform : public virtual Component {
public:
	Transform() {}
	~Transform() {}

	virtual void awake() {}
	virtual void start() {}
	virtual void prepare() {}
	virtual void update() {}
	virtual void late_update() {}
	virtual void shutdown() {}

	Actor* m_actor = nullptr;
	glm::vec3 m_position = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 m_rotation = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 m_scale = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::mat4 m_model = glm::mat4(1.0f);

};

class Mesh : public virtual Component {
public:

	virtual void awake() {}
	virtual void start() {}
	virtual void prepare() {}
	virtual void update() {}
	virtual void late_update() {}
	virtual void shutdown() {}

	uint32_t m_index_id = 0;
	uint32_t m_vertex_id = 0;
	uint32_t m_index_count = 0;
	uint32_t m_vao_id = 0;

	std::vector<uint32_t> m_index_data;
	std::vector<glm::vec3> m_vertex_data;
};

class Actor : public virtual Component {
public:
	Actor() {}

	~Actor() {
		if( m_tranform != nullptr) delete m_tranform;
		if (m_mesh != nullptr) delete m_mesh;

		m_tranform = nullptr;
		m_mesh = nullptr;
	}

	virtual void awake() {}
	virtual void start() {}
	virtual void prepare() {}
	virtual void update() {}
	virtual void late_update() {}
	virtual void shutdown() {}

	void create(
		std::string obj_path, 
		int32_t program_id,
		glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3 rot = glm::vec3(0.0f, 0.0f, 0.0f), 
		glm::vec3 scl = glm::vec3(1.0f, 1.0f, 1.0f)
	) {
		m_tranform = new Transform();

		m_tranform->m_position = pos; 
		m_tranform->m_rotation = rot;
		m_tranform->m_scale = scl;

		m_mesh = new Mesh();
		_load_obj(obj_path, m_mesh);

		this->m_program_id = program_id;

		auto hierarchy = Hierarchy::get_instance();
		hierarchy->add(this);

	}

	//temp?
	void bind_model() {

		// quick implementation. double check order.
		// almost might be more correct to have this in the Trasform
		m_tranform->m_model = glm::mat4(1.0f);
		m_tranform->m_model = glm::translate(m_tranform->m_model, m_tranform->m_position);
		m_tranform->m_model = glm::rotate(m_tranform->m_model, m_tranform->m_rotation.x, glm::vec3(1.0f, 0.0f, 0.0f) );
		m_tranform->m_model = glm::rotate(m_tranform->m_model, m_tranform->m_rotation.y, glm::vec3(0.0f, 1.0f, 0.0f) );
		m_tranform->m_model = glm::rotate(m_tranform->m_model, m_tranform->m_rotation.z, glm::vec3(0.0f, 0.0f, 1.0f) );
		m_tranform->m_model = glm::scale(m_tranform->m_model, m_tranform->m_scale);

		int32_t m_location = glGetUniformLocation(m_program_id, "model");
		if (m_location == -1)    std::cout << "Model matrix not found in shader!" << std::endl;
		glUniformMatrix4fv(m_location, 1, false, &m_tranform->m_model[0][0]);
	}

	bool active() { return m_active; }
	int32_t program() { return m_program_id; }

	bool       m_active = true;
	Transform* m_tranform = nullptr;
	Mesh*      m_mesh = nullptr;
	// change this to a material class
	int32_t    m_program_id = 0;
private:

};
