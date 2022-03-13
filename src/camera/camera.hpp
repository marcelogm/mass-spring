#pragma once
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>

using glm::mat4;
using glm::vec3;

class Camera {
private:
	vec3 position;
	vec3 front;
	vec3 right;
	vec3 up;
	mat4 view;
	float yaw;
	float pitch;
public:
	void look(float, float);
	void update();
	void goFoward(float);
	void goBack(float);
	void goRight(float);
	void goLeft(float);
	void goUp(float);
	void goDown(float);
	mat4 getView();
	vec3 getPosition();
	Camera(vec3, vec3, float, float);
	Camera();
};