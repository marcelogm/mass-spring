#pragma once
#include "../entity/entity.hpp"
#include "../scene/scene.hpp"
#include "../configuration/config.hpp"

using glm::vec3;
using std::map;

class ExternalForce {
private:
	const float ACCELERATION = 9.80665f;
public:
	void apply(Entity* entity, float delta_t);
};

class Frametime {
private:
	static Frametime* instance;
	Frametime();

	double framerate;
	double frametime;

	double lastFrameTime = 0.0;
	double currentFrameTime = 0.0;
	double timeFrameDiff = 0.0;
	size_t frameCounter = 0;
public:
	static Frametime* getInstance();
	void countFrame();
	double getFramerate();
	double getFrametime();
	double getDelta();
};

class Simulator {
private:
	using ColliderProvider = std::function<vector<Collideable*>*(Entity*)>;
	// TODO: todas esses métodos poderiam ser classes que seguem o single responsability principe
	vec3 calculateForces(vec3 x, vec3 y, float rest, float stiffness);
	vector<Entity*> getCollideablesFromBroadPhase(vector<Entity*>);
	void collideAtNarrowPhase(vector<Entity*>, float);

	static map<Collideable*, Entity*> createCollisionPool(vector<Entity*>, ColliderProvider);
	static bool definitelyLessThan(float, float, float);
	static vec3 integrate(Particle*, float, float);

	ExternalForce* externalForces;
	size_t iterations = 25;
public:
	void update(Scene* scene);
	void updateEntity(Entity* entity, const float timestep, const float damping);                                                                                                                                        void getSpringForces(vec3 x, vec3 y, float rest, float stiffness);
	Simulator();
};