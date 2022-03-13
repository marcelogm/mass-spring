#include "gui.hpp"

void CommandService::onKeyPress(int key, int action) {
	for (auto strategy : this->strategies) {
		if (strategy->matches(key, action)) {
			strategy->apply();
		}
	}
}

void CommandService::onMouseMove(double, double) {
	auto view = camera->getView();
	int a = 1;
}

void CommandService::setCamera(Camera* camera) {
	this->camera = camera;
	this->strategies = {
		new MoveUp(camera),
		new MoveDown(camera),
		new MoveLeft(camera),
		new MoveRight(camera),
		new MoveFront(camera),
		new MoveBack(camera),
		new YawLeft(camera),
		new YawRight(camera),
		new PitchDown(camera),
		new PitchUp(camera)
	};
}

CommandService* CommandService::getInstance() {
	if (instance == nullptr) {
		instance = new CommandService();
	}
	return instance;
}

CommandService::CommandService() {
	this->strategies = {};
}

CommandService* CommandService::instance = nullptr;