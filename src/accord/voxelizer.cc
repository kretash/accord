#include "accord/voxelizer.h"
#include "accord/hierarchy.h"

void Voxelizer::generate( std::string other ) {
	m_clone = _load_obj( other );

	m_buffer.resize( size );
	for( int32_t i = 0; i < size; ++i ) {
		m_buffer[i].resize( size );
		for( int32_t e = 0; e < size; ++e ) {
			m_buffer[i][e].resize( size );
		}
	}


	for( int32_t i = 0; i < m_buffer.size(); ++i ){
		for( int32_t e = 0; e < m_buffer[i].size(); ++e ) {
			for( int32_t o = 0; o < m_buffer[i][e].size(); ++o ) {

				uint8_t* c = &m_buffer[i][e][o];

				*c = i == e || i == o || e == o;

			}
		}
	}

	//std::shared_ptr<Hierarchy> hierarchy = Hierarchy::get_instance();

	for (int32_t i = 0; i < m_buffer.size(); ++i) {
		for (int32_t e = 0; e < m_buffer[i].size(); ++e) {
			for (int32_t o = 0; o < m_buffer[i][e].size(); ++o) {

				uint8_t* c = &m_buffer[i][e][o];

				drawable* cube = new drawable();

				//_load_obj( "cube.obj", cube );
				cube->active = *c != 0;
				cube->model = glm::translate(glm::mat4(1.0f), glm::vec3(i, e, o));

				//hierarchy->add(cube);
				m_cubes.push_back(cube);

			}
		}
	}

}

void Voxelizer::draw(GLint program){
	
}

