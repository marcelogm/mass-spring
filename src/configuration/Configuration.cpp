#include "config.hpp"

Configuration::Configuration() {
	this->viewport = { 1200.0f, 800.0f };
	this->light = { vec4(5.0f, 10.0f, 2.0f, 1.0f), 0.5f, 0.1f };
	this->params = { 1.00f, 2.00f, glm::vec3(-1.f, 0.f, 0.f), 5, 0.5, 0.1, 0.0001f, 5, false, false };
	this->openGL = { RenderMode::TRIANGLES };
	this->debug = { false, false, false, glm::vec3(0.f, 0.f, -0.f), 1.2f, 0.002f, };
};

Configuration* Configuration::getInstance() {
	if (instance == nullptr) {
		instance = new Configuration();
	}
	return instance;
}

Configuration* Configuration::instance = nullptr;

ViewportDimension* Configuration::getViewport() {
	return &this->viewport;
}

LightConfiguration* Configuration::getLight() {
	return &this->light;
}

SimulationParams* Configuration::getSimulationParams() {
	return &this->params;
}

OpenGLConfiguration* Configuration::getOpenGLConfiguration() {
	return &this->openGL;
}

Debug* Configuration::getDebug() {
	return &this->debug;
}
