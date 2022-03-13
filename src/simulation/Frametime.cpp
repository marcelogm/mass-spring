#include "simulation.hpp"

Frametime::Frametime() {
};

Frametime* Frametime::getInstance() {
	if (instance == nullptr) {
		instance = new Frametime();
	}
	return instance;
}

void Frametime::countFrame() {
	currentFrameTime = glfwGetTime();
	timeFrameDiff = currentFrameTime - lastFrameTime;
	frameCounter++;
	if (timeFrameDiff >= 1.0 / 30.0) {
		framerate = ((1.0 / timeFrameDiff) * frameCounter);
		frametime = (timeFrameDiff / frameCounter) * 1000;
		lastFrameTime = currentFrameTime;
		frameCounter = 0;
	}
}

double Frametime::getFramerate() {
	return this->framerate;
}

double Frametime::getFrametime() {
	return this->frametime;
}

double Frametime::getDelta() {
	if (this->timeFrameDiff == INFINITE) {
		return 0.1;
	}
	return this->timeFrameDiff;
}

Frametime* Frametime::instance = nullptr;