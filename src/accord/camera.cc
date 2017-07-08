#include "accord/context.h"
#include "accord/camera.h"
#include "accord/hierarchy.h"

void Camera::render(GLint program){
	
	glClearColor( 0.5f, 0.5f, 1.0f, 1.0f );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glEnable( GL_DEPTH_TEST );

	auto renderer = Hierarchy::get_instance();
	std::vector<drawable*> render_bin = renderer->m_render_bin;

	auto context = Context::get_instance();
	render_state render_state = context->m_render_state;

	for( int i = 0; i < render_bin.size(); ++i ) {

		drawable * c = render_bin[i];

		if (c->active == false) continue;

		if( render_state.bound_program != program ) {

			glUseProgram( program );

			GLint view_location = glGetUniformLocation(program, "view");
			GLint proj_location = glGetUniformLocation(program, "projection");

			glUniformMatrix4fv(view_location, 1, false, &m_view[0][0]);
			glUniformMatrix4fv(proj_location, 1, false, &m_proj[0][0]);

			render_state.bound_program = program;
		}

		if( render_state.bound_vao != render_bin[i]->vao ) {
			glBindVertexArray( render_bin[i]->vao );
			render_state.bound_vao = render_bin[i]->vao;
		}

		if( render_state.bound_vbuffer != render_bin[i]->v_buffer ) {
			glBindBuffer( GL_ARRAY_BUFFER, render_bin[i]->v_buffer );
			render_state.bound_vbuffer = render_bin[i]->v_buffer;
		}

		if( render_state.bound_ebuffer != render_bin[i]->e_buffer ) {
			glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, render_bin[i]->e_buffer );
			render_state.bound_ebuffer = render_bin[i]->e_buffer;
		}

		render_bin[i]->bind_model( program );

		glDrawElements( GL_TRIANGLES, render_bin[i]->e_count, GL_UNSIGNED_INT, ( void* ) 0 );

	}


}