
#define TEMPLATE_FUNCTIONS
#include "accord/accord.h"
#include "accord/context.h"
#include "accord/gl_helper.h"
#include "accord/actor.h"
#include "accord/input.h"
#include "accord/editor.h"
#include "accord/camera.h"

int main(int argc, char** argv) {
	
	auto context = Context::get_instance();

	context->awake();

	GLint program = _create_program("vs_basic.glsl", "fs_basic.glsl");
	if (program == -1) return 1;
	glUseProgram(program);

	std::shared_ptr<Actor> teapot = std::make_shared<Actor>();
	teapot->create("teapot.obj", program);

	std::shared_ptr<Actor> plane = std::make_shared<Actor>();
	plane->create("plane.obj", program);

	context->start();
	context->prepare();

	std::shared_ptr<Camera> camera = std::make_shared<Camera>();
	Input* input = get_component<Input>();

	while (context->running) {

		context->update();
		
		camera->render();

		context->swap();
	}

	context->shutdown();
	return 0;
}
