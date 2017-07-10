#pragma once

#include <memory>
#include <vector>

#include "accord/accord.h"

class Hierarchy : public virtual Component {
public:
	~Hierarchy() {}

	void add(Actor* const d ) {
		m_render_bin.push_back( d );
	}

	std::vector<Actor*> get_render_bin() const {
		return m_render_bin;
	}

	virtual void awake() {}
	virtual void start() {}
	virtual void prepare() {}
	virtual void update() {}
	virtual void late_update() {}
	virtual void shutdown() {}

private:
	Hierarchy(){}

	friend Context;
	std::vector<Actor*> m_render_bin;

};
