#pragma once

#include <memory>
#include <vector>

#include "accord/accord.h"

class Camera;
class Actor;

class Hierarchy{
public:

	static std::shared_ptr<Hierarchy> get_instance(){
		static std::shared_ptr<Hierarchy> m_instance( new Hierarchy );
		return m_instance;
	}

	~Hierarchy() {}

	void add(Actor* d ){
		m_render_bin.push_back( d );
	}

protected:
	Hierarchy(){}
private:

	friend Camera;
	std::vector<Actor*> m_render_bin;

};
