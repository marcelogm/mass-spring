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
	void resetRenderMode();
	void prepare();
	void render(Entity*, Camera*, LightConfiguration*);
};

class Scene {
private:
	using DebugModelProvider = std::function<mat4()>;
	vector<Entity*> entities;
	Entity* debug;
	Renderer* renderer;
	Camera* camera;
	DebugModelProvider provider;
public:
	Scene(vector<Entity*> entities, Entity* debug, DebugModelProvider provider, Camera* camera);
	void renderDebug(vector<Entity>* entities, Camera* camera);
	void render();
	Camera* getCamera();
	vector<Entity*> getEntities();
};

class ClothSceneFactory {
public:
	Scene* build();
	Entity* getSphere(vec3 position, float scale);
};