#include "main.hpp"

Application::Application() {
	this->window = new WindowEngine();
	glGenVertexArrays(1, &vertexArray);
	glBindVertexArray(vertexArray);
	glEnable(GL_DEPTH_TEST);
	glCullFace(GL_BACK);
}

void Application::loop() {
	Simulator simulator = Simulator();
	Scene* scene = ClothSceneFactory().build();
	CommandService::getInstance()->setCamera(scene->getCamera());
	while (this->window->shouldClose()) {
		simulator.update(scene);
		scene->render();
		this->window->render();
		Frametime::getInstance()->countFrame();
	}
}

void Application::detroy() {
	// kill them all
}

