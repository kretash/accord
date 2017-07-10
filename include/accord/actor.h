#pragma once

#include "accord/accord.h"
#include "accord/context.h"
#include "accord/hierarchy.h"
#include "accord/material.h"

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

	glm::mat4 get_model_mat() {
		if (m_dirty) {
			m_model = glm::mat4(1.0f);
			m_model = glm::translate(m_model, m_position);
			m_model = glm::rotate(m_model, m_rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
			m_model = glm::rotate(m_model, m_rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
			m_model = glm::rotate(m_model, m_rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
			m_model = glm::scale(m_model, m_scale);
			m_dirty = false;
		}
		return m_model;
	}

	void set_position( glm::vec3 pos ) {
		m_dirty = true;
		m_position = pos;
	}

	void set_rotation(glm::vec3 rot) {
		m_dirty = true;
		m_rotation = rot;
	}

	void set_scale(glm::vec3 scl) {
		m_dirty = true;
		m_scale = scl;
	}

	glm::vec3 get_position() const { return m_position; }
	glm::vec3 get_rotation() const { return m_rotation; }
	glm::vec3 get_scale() const { return m_scale; }

	Actor* m_actor = nullptr;
private:
	bool      m_dirty    = true;
	glm::vec3 m_position = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 m_rotation = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 m_scale    = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::mat4 m_model    = glm::mat4(1.0f);

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
		const std::string obj_path, 
		const std::shared_ptr<Material> material,
		const glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f),
		const glm::vec3 rot = glm::vec3(0.0f, 0.0f, 0.0f), 
		const glm::vec3 scl = glm::vec3(1.0f, 1.0f, 1.0f)
	) {

		m_tranform = new Transform();
		m_tranform->set_position( pos ); 
		m_tranform->set_rotation( rot );
		m_tranform->set_scale( scl );

		m_mesh = new Mesh();
		_load_obj(obj_path, m_mesh);

		m_material = material;

		auto ctx = Context::get_instance();
		auto hierarchy = ctx->get_hierarchy();
		hierarchy->add(this);

	}

	void bind() const {
		auto mat = m_tranform->get_model_mat();
		m_material->bind_model(mat);
	}

	bool                         active() { return m_active; }
	std::shared_ptr<Material>    get_material() { return m_material; }

	bool                         m_active = true;
	Transform*                   m_tranform = nullptr;
	Mesh*                        m_mesh = nullptr;
	std::shared_ptr<Material>    m_material;
private:

};
