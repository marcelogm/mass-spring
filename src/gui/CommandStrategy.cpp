#include "gui.hpp"

CommandStrategy::CommandStrategy(Camera* camera) {
	this->camera = camera;
}

bool CommandStrategy::isPressing(int action) {
	return (action == GLFW_PRESS || action == GLFW_REPEAT);
}

bool MoveLeft::matches(int key, int action) {
	return key == GLFW_KEY_LEFT && isPressing(action);
};

void MoveLeft::apply() {
	camera->goLeft(1.f);
};

bool MoveRight::matches(int key, int action) {
	return  key == GLFW_KEY_RIGHT && isPressing(action);
};

void MoveRight::apply() {
	camera->goRight(1.0f);
};

bool MoveFront::matches(int key, int action) {
	return  key == GLFW_KEY_UP && isPressing(action);
};

void MoveFront::apply() {
	camera->goFoward(1.0f);
};

bool MoveBack::matches(int key, int action) {
	return key == GLFW_KEY_DOWN && isPressing(action);
};

void MoveBack::apply() {
	camera->goBack(1.0f);
};

bool MoveUp::matches(int key, int action) {
	return key == GLFW_KEY_Z && isPressing(action);
};

void MoveUp::apply() {
	camera->goUp(1.0f);
};

bool MoveDown::matches(int key, int action) {
	return key == GLFW_KEY_X && isPressing(action);
};

void MoveDown::apply() {
	camera->goDown(1.0f);
};

bool YawLeft::matches(int key, int action) {
	return key == GLFW_KEY_A && isPressing(action);
};

void YawLeft::apply() {
	camera->look(-1.0f, 0.0f);
};

bool YawRight::matches(int key, int action) {
	return key == GLFW_KEY_D && isPressing(action);
};

void YawRight::apply() {
	camera->look(1.0f, 0.0f);
};


bool PitchUp::matches(int key, int action) {
	return key == GLFW_KEY_W && isPressing(action);
};

void PitchUp::apply() {
	camera->look(0.0f, 1.0f);
};

bool PitchDown::matches(int key, int action) {
	return key == GLFW_KEY_S && isPressing(action);
};

void PitchDown::apply() {
	camera->look(0.0f, -1.0f);
};