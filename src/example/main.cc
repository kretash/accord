

#include <Windows.h>
#include <iostream>
#include <math.h>
#include <iostream>
#include <fstream>

#define TEMPLATE_FUNCTIONS
#include "accord/context.h"
#include "accord/gl_helper.h"
#include "accord/input.h"
#include "accord/camera.h"
#include "accord/voxelizer.h"
#include "accord/hierarchy.h"

int main(int argc, char** argv) {
	
	auto context = Context::get_instance();
	auto hierarchy = Hierarchy::get_instance();

	context->awake();

	drawable teapot;
	_load_obj("teapot.obj", &teapot);
	hierarchy->add( &teapot );

	drawable plane;
	_load_obj("plane.obj", &plane);
	hierarchy->add( &plane );

	Voxelizer vox;
	vox.generate("teapot.obj");

	GLint program = _create_program("vs_basic.glsl", "fs_basic.glsl");
	if (program == -1) return 1;
	glUseProgram(program);

	context->start();
	context->prepare();

	std::shared_ptr<Camera> camera = std::make_shared<Camera>();
	Input* input = get_component<Input>();

	while (context->running) {

		camera->update();
		context->update();
		
		camera->render(program);
		context->render();

		//
		//

	}

	context->shutdown();
	//
	return 0;
}
