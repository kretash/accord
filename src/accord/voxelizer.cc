#include "accord/voxelizer.h"
#include "accord/actor.h"

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
  
	for (int32_t i = 0; i < m_buffer.size(); ++i) {
		for (int32_t e = 0; e < m_buffer[i].size(); ++e) {
			for (int32_t o = 0; o < m_buffer[i][e].size(); ++o) {

				uint8_t* c = &m_buffer[i][e][o];

				Actor* cube = new Actor();

				_load_obj( "cube.obj", cube->m_mesh );
				cube->m_active = *c != 0;
				cube->m_tranform->set_position( glm::vec3(i, e, o) );

			}
		}
	}

}

void Voxelizer::draw(GLint program){
	
}

