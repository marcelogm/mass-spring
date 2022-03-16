#pragma once 

#include <glm/glm.hpp>
#include <functional>

using glm::vec3;

enum class CollideableType { POINT, SPHERE, BOX };

class Collideable {
public:
	using PositionProvider = std::function<vec3()>;
	virtual CollideableType getType() = 0;
	virtual void update() = 0;
};

class CollideableBoundingBox : public Collideable {
private:
	PositionProvider minProvider;
	PositionProvider maxProvider;
	vec3 min;
	vec3 max;
public:
	CollideableBoundingBox(PositionProvider minProvider, PositionProvider maxProvider);
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