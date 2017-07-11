#include <memory>

#define TEMPLATE_FUNCTIONS
#include "accord/accord.h"
#include "accord/context.h"
#include "accord/actor.h"
#include "accord/camera.h"

class example {
public:
	example() {}
	~example() {}

	void awake() {}

	void start() {

		material = std::make_shared<Material>();
		material->create("vs_basic.glsl", "fs_basic.glsl");

		teapot = std::make_shared<Actor>();
		teapot->create("teapot.obj", material);

		plane = std::make_shared<Actor>();
		plane->create("plane.obj", material);

	}

	void prepare() {
		camera = std::make_shared<Camera>();
	}

	void update() {}
	void late_update() {}

	void render() {
		camera->render();
	}

	void shutdown() {}

	std::shared_ptr<Camera> camera;
	std::shared_ptr<Material> material;
	std::shared_ptr<Actor> teapot;
	std::shared_ptr<Actor> plane;
};

int main(int argc, char** argv) {

	auto context = Context::get_instance();

	std::shared_ptr<example> my_example = std::make_shared<example>();

	context->set_awake_function(std::bind(&example::awake, my_example.get()));
	context->set_start_function(std::bind(&example::start, my_example.get()));
	context->set_prepare_function(std::bind(&example::prepare, my_example.get()));
	context->set_update_function(std::bind(&example::update, my_example.get()));
	context->set_late_update_function(std::bind(&example::late_update, my_example.get()));
	context->set_render_function(std::bind(&example::render, my_example.get()));
	context->set_shutdown_function(std::bind(&example::shutdown, my_example.get()));

	int32_t err = context->run();

	return err;
}
