#pragma once

#include "../configuration/config.hpp"
#include "../camera/camera.hpp"
#include "../entity/entity.hpp"

using glm::mat4;
using std::vector;

class Renderer {
private:
	float aspect;
	mat4 projection;
	float lightPosition;
	float ambientLightStrength;
	float diffuseLightStrength;
public:
	void clear();
	void prepare();
	void render(Entity*, Camera*, LightConfiguration*);
};

class Scene {
private:
	vector<Entity*> entities;
	Renderer* renderer;
	Camera* camera;
public:
	Scene(vector<Entity*> entities, Camera* camera);
	void render();
	Camera* getCamera();
	vector<Entity*> getEntities();
};

class ClothSceneFactory {
public:
	Scene* build();
	Entity* getDebugBox(vec3 position, float scale, vector<ShaderInfo> shaders);
};