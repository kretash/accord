
#define TEMPLATE_FUNCTIONS
#include "accord/accord.h"
#include "accord/context.h"
#include "accord/actor.h"
#include "accord/camera.h"

int main(int argc, char** argv) {
	
	auto context = Context::get_instance();

	context->awake();

	std::shared_ptr<Material> material = std::make_shared<Material>();
	material->create("vs_basic.glsl", "fs_basic.glsl");

	std::shared_ptr<Actor> teapot = std::make_shared<Actor>();
	teapot->create("teapot.obj", material);

	std::shared_ptr<Actor> plane = std::make_shared<Actor>();
	plane->create("plane.obj", material);

	context->start();
	context->prepare();

	std::shared_ptr<Camera> camera = std::make_shared<Camera>();

	while (context->running) {

		context->update();
		
		camera->render();

		context->swap();
	}

	context->shutdown();
	return 0;
}
