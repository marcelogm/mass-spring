#include "collision.hpp"

CollideableBoundingBox::CollideableBoundingBox(PositionProvider minProvider, PositionProvider maxProvider) {
	this->minProvider = minProvider;
	this->maxProvider = maxProvider;
}

CollideableType CollideableBoundingBox::getType() {
	return CollideableType::BOX;
}

void CollideableBoundingBox::update() {
	this->min = this->minProvider();
	this->max = this->maxProvider();
}

vec3 CollideableBoundingBox::getMin() {
	return this->min;
}

vec3 CollideableBoundingBox::getMax() {
	return this->max;
}

float CollideableBoundingBox::isIntersectedBy(CollideableBoundingBox* box) {
	auto itMin = this->getMin();
	auto itMax = this->getMax();
	auto otherMin = box->getMin();
	auto otherMax = box->getMax();

	return (itMin.x <= otherMax.x && itMax.x >= otherMin.x) &&
		(itMin.y <= otherMax.y && itMax.y >= otherMin.y) &&
		(itMin.z <= otherMax.z && itMax.z >= otherMin.z);
}


CollideableBoudingSphere::CollideableBoudingSphere(PositionProvider provider, float radius) {
	this->provider = provider;
	this->radius = radius;
}

CollideableType CollideableBoudingSphere::getType() {
	return CollideableType::SPHERE;
}

void CollideableBoudingSphere::update() {
	this->position = provider();
}

vec3 CollideableBoudingSphere::getPosition() {
	return this->position;
}

float CollideableBoudingSphere::getRadius() {
	return this->radius;
}

float CollideableBoudingSphere::isIntersectedBy(CollideableBoudingSphere* sphere) {
	auto it = this->getPosition();
	auto other = sphere->getPosition();
	auto distance = glm::sqrt(
		(it.x - other.x) * (it.x - other.x) +
		(it.y - other.y) * (it.y - other.y) +
		(it.z - other.z) * (it.z - other.z)
	);
	// is it right?
	return distance < (this->getRadius() + sphere->getRadius());
}

float CollideableBoudingSphere::isIntersectedBy(CollideableBoundingBox* box) {
	auto sphere = this->getPosition();
	auto boxMin = box->getMin();
	auto boxMax = box->getMax();
#undef max
#undef min
	auto closest = vec3(
		glm::max(boxMin.x, glm::min(sphere.x, boxMax.x)),
		glm::max(boxMin.y, glm::min(sphere.y, boxMax.y)),
		glm::max(boxMin.z, glm::min(sphere.z, boxMax.z))
	);

	auto distance = glm::sqrt(
		(closest.x - sphere.x) * (closest.x - sphere.x) +
		(closest.y - sphere.y) * (closest.y - sphere.y) +
		(closest.z - sphere.z) * (closest.z - sphere.z)
	);
	// is it right?
	return distance < this->getRadius();
}

CollideablePoint::CollideablePoint(PositionProvider provider) {
	this->provider = provider;
}

CollideableType CollideablePoint::getType() {
	return CollideableType::POINT;
}

void CollideablePoint::update() {
	this->position = provider();
}

vec3 CollideablePoint::getPosition() {
	return this->position;
}

float CollideablePoint::isInside(CollideableBoudingSphere* sphere) {
	auto point = this->getPosition();
	auto other = sphere->getPosition();
	auto distance = glm::sqrt(
		(point.x - other.x) * (point.x - other.x) +
		(point.y - other.y) * (point.y - other.y) +
		(point.z - other.z) * (point.z - other.z)
	);
	// is it right?
	return distance < sphere->getRadius();
}

float CollideablePoint::isInside(CollideableBoundingBox* box) {
	auto point = this->getPosition();
	auto minBox = box->getMin();
	auto maxBox = box->getMax();

	if ((point.x >= minBox.x && point.x <= maxBox.x) &&
		(point.y >= minBox.y && point.y <= maxBox.y) &&
		(point.z >= minBox.z && point.z <= maxBox.z)) {
		return 1.0f;
	}
	return 0.0f;
}

float CollisionChecker::isColliding(Collideable* it, Collideable* other) {
	// Eu deveria conseguir fazer isso com uma interface
	if (it->getType() == CollideableType::SPHERE) {
		auto sphere = dynamic_cast<CollideableBoudingSphere*>(it);
		if (other->getType() == CollideableType::SPHERE) {
			return sphere->isIntersectedBy(dynamic_cast<CollideableBoudingSphere*>(other));
		} else {
			return sphere->isIntersectedBy(dynamic_cast<CollideableBoundingBox*>(other));
		}
	} else if (it->getType() == CollideableType::BOX) {
		auto box = dynamic_cast<CollideableBoundingBox*>(it);
		return box->isIntersectedBy(dynamic_cast<CollideableBoundingBox*>(other));
	} else {
		auto point = dynamic_cast<CollideablePoint*>(it);
		if (other->getType() == CollideableType::SPHERE) {
			return point->isInside(dynamic_cast<CollideableBoudingSphere*>(other));
		} else {
			return point->isInside(dynamic_cast<CollideableBoundingBox*>(other));
		}
	}
}
