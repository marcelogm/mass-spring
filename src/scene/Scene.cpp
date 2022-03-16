#include "scene.hpp"

Scene::Scene(vector<Entity*> entities, Entity* debug, DebugModelProvider provider, Camera* camera) {
	this->entities = entities;
	this->entities.push_back(debug);
	this->debug = debug;
	this->camera = camera;
	this->renderer = new Renderer();
	this->provider = provider;
}

void Scene::render() {
	this->renderer->clear();
    this->renderer->prepare();

	// horrivel
	debug->updateModel(provider());

	for (Entity* entity : this->entities) {
		entity->update();
        this->renderer->render(entity, this->camera, Configuration::getInstance()->getLight());
		for (Entity collision : *entity->getCollisionEntities()) {
			collision.update();
			glPolygonMode(GL_FRONT, GL_LINE);
			glPolygonMode(GL_BACK, GL_LINE);
			this->renderer->render(&collision, this->camera, Configuration::getInstance()->getLight());
			glPolygonMode(GL_FRONT, GL_FILL);
			glPolygonMode(GL_BACK, GL_FILL);
		}
	}
}

Camera* Scene::getCamera() {
	return camera;
}

vector<Entity*> Scene::getEntities() {
	return this->entities;
}

