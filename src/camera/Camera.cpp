#include "camera.hpp"

Camera::Camera(vec3 position, vec3 front, float yaw, float pitch) {
	this->position = position;
	this->front = front;
	this->up = vec3(0.0f, 1.0f, 0.0f);
	this->yaw = yaw;
	this->pitch = pitch;
	this->update();
}

Camera::Camera() {
	this->position = vec3(0.0f, 0.0f, -1.0f);
	this->up = vec3(0.0f, 1.0f, 0.0f);
	this->yaw = -90.0f;
	this->pitch = 0.0f;
	this->front = vec3();
	this->update();
}

void Camera::look(float yawOffset, float pitchOffset) {
	pitch += pitchOffset;
	yaw += yawOffset;
	if (pitch > 89.0f) {
		pitch = 89.0f;
	}
	if (pitch < -89.0f) {
		pitch = -89.0f;
	}
	this->update();
}

void Camera::update() {
	auto yawRadians = glm::radians(yaw);
	auto pitchRadians = glm::radians(pitch);

	front.x = glm::cos(yawRadians) * glm::cos(pitchRadians);
	front.y = glm::sin(pitchRadians);
	front.z = glm::sin(yawRadians) * glm::cos(pitchRadians);

	this->front = glm::normalize(this->front);
	this->right = glm::normalize(glm::cross(front, up));
	this->view = glm::lookAt(position, position + front, up);
}

mat4 Camera::getView() {
	return this->view;
}

vec3 Camera::getPosition() {
	return this->position;
}

void Camera::goFoward(float speed) {
	position += front * speed;
	this->update();
}

void Camera::goBack(float speed) {
	position -= front * speed;
	this->update();
}

void Camera::goRight(float speed) {
	position += right * speed;
	this->update();
}

void Camera::goLeft(float speed) {
	position -= right * speed;
	this->update();
}

void Camera::goUp(float speed) {
	position += up * speed;
	this->update();
}

void Camera::goDown(float speed) {
	position -= up * speed;
	this->update();
}