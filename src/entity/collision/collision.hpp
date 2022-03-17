#pragma once 

#include <glm/glm.hpp>
#include <functional>
#include <vector>

using glm::vec3;
using std::vector;

enum class CollideableType { POINT, SPHERE, BOX };

typedef struct Particle {
	size_t i;
	vec3 velocity;
	vec3 force;
	float mass;
};

typedef struct Spring {
	size_t i;
	size_t j;
	float restLength;
};

class Collideable {
public:
	using AffectedParticlesProvider = std::function<vector<Particle*>*()>;
	using PositionProvider = std::function<vec3()>;
	virtual CollideableType getType() = 0;
	virtual void update() = 0;
	vector<Particle*>* affectedParticles;
};

class CollideableBoundingBox : public Collideable {
private:
	PositionProvider minProvider;
	PositionProvider maxProvider;
	vec3 min;
	vec3 max;
public:
	CollideableBoundingBox(PositionProvider minProvider, PositionProvider maxProvider);
	CollideableBoundingBox(PositionProvider minProvider, PositionProvider maxProvider, AffectedParticlesProvider affected);
	CollideableType getType() override;
	void update();
	vec3 getMin();
	vec3 getMax();

	float isIntersectedBy(CollideableBoundingBox* box);
};

class CollideableBoudingSphere: public Collideable {
private:
	PositionProvider provider;
	vec3 position;
	float radius;
public:
	CollideableBoudingSphere(PositionProvider provider, float radius);
	CollideableBoudingSphere(PositionProvider provider, float radius, AffectedParticlesProvider affected);
	CollideableType getType() override;
	void update();
	vec3 getPosition();
	float getRadius();

	float isIntersectedBy(CollideableBoudingSphere* sphere);
	float isIntersectedBy(CollideableBoundingBox* box);
};

class CollideablePoint : public Collideable {
private:
	PositionProvider provider;
	vec3 position;
public:
	CollideablePoint(PositionProvider provider);
	CollideablePoint(PositionProvider provider, AffectedParticlesProvider affected);
	CollideableType getType() override;
	void update();
	vec3 getPosition();

	float isInside(CollideableBoudingSphere* sphere);
	float isInside(CollideableBoundingBox* box);
};

class CollisionChecker {
public:
	float isColliding(Collideable* it, Collideable* other);
};
