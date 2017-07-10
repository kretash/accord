#pragma once

#include <vector>
#include "accord/accord.h"
#include "accord/context.h"

class Material : public virtual Component {
public:
	Material();
	~Material();
	bool create(std::string vs, std::string fs);
	void bind_model(glm::mat4 model_mat);

	int32_t get_program() { return m_program_id; }

	virtual void awake() {}
	virtual void start() {}
	virtual void prepare() {}
	virtual void update() {}
	virtual void late_update() {}
	virtual void shutdown() {}

private:
	int32_t m_program_id = -1;
};

class MaterialLibrary : public virtual Component {
public:
	MaterialLibrary() {}
	~MaterialLibrary() {}

	void add(Material* mat) {
		m_materials.push_back(mat);
	}

	void remove(Material* mat) {
		// ugg
		m_materials.erase(std::remove_if(m_materials.begin(), m_materials.end(),
			[&](Material* cmat) { return cmat == mat; }), m_materials.end());
	}

	virtual void awake() {}
	virtual void start() {}
	virtual void prepare() {}
	virtual void update() {}
	virtual void late_update() {}
	virtual void shutdown() {}

private:
	std::vector<Material*> m_materials;
};