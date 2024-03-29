#include "accord/context.h"
#include "accord/camera.h"
#include "accord/hierarchy.h"
#include "accord/actor.h"

void Camera::render(){
	
	glClearColor( 0.5f, 0.5f, 1.0f, 1.0f );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glEnable( GL_DEPTH_TEST );

	auto ctx = Context::get_instance();
	auto hierarchy = ctx->get_hierarchy();
	std::vector<Actor*> render_bin = hierarchy->get_render_bin();

	auto context = Context::get_instance();
	render_state render_state = context->m_render_state;

	for( int i = 0; i < render_bin.size(); ++i ) {

		Actor* c = render_bin[i];

		if (c->active() == false) continue;

		// move this elsewhere??
		int32_t program = c->get_material()->get_program();
		if( render_state.bound_program != program ) {

			glUseProgram( program );

			GLint view_location = glGetUniformLocation( program, "view");
			GLint proj_location = glGetUniformLocation( program, "projection");

			glUniformMatrix4fv(view_location, 1, false, &m_view[0][0]);
			glUniformMatrix4fv(proj_location, 1, false, &m_proj[0][0]);

			render_state.bound_program = program;
		}

		if( render_state.bound_vao != render_bin[i]->m_mesh->m_vao_id ) {
			glBindVertexArray( render_bin[i]->m_mesh->m_vao_id );
			render_state.bound_vao = render_bin[i]->m_mesh->m_vao_id;
		}

		if( render_state.bound_vbuffer != render_bin[i]->m_mesh->m_vertex_id) {
			glBindBuffer( GL_ARRAY_BUFFER, render_bin[i]->m_mesh->m_vertex_id);
			render_state.bound_vbuffer = render_bin[i]->m_mesh->m_vertex_id;
		}

		if( render_state.bound_ebuffer != render_bin[i]->m_mesh->m_index_id) {
			glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, render_bin[i]->m_mesh->m_index_id);
			render_state.bound_ebuffer = render_bin[i]->m_mesh->m_index_id;
		}

		render_bin[i]->bind();

		glDrawElements( GL_TRIANGLES, render_bin[i]->m_mesh->m_index_count, GL_UNSIGNED_INT, ( void* ) 0 );

	}


}