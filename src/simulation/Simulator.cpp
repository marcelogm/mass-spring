#include "simulation.hpp"
#include "simulation.hpp"

bool Simulator::definitelyLessThan(float a, float b, float epsilon) {
	return (b - a) > ((fabs(a) < fabs(b) ? fabs(b) : fabs(a)) * epsilon);
}

vec3 Simulator::integrate(Particle* particle, float timestep, float damping) {
	particle->velocity = particle->velocity + (particle->force / (particle->mass * timestep));
	particle->velocity *= damping;
	particle->force = vec3();
	return (particle->velocity * timestep);
}

vec3 Simulator::calculateForces(vec3 x, vec3 y, float rest, float stiffness) {
	auto spring = x - y;
	auto length = glm::length(spring);
	auto distance = length - rest;
	auto h = -stiffness * distance;
	return spring * h;
}

map<Collideable*, Entity*> Simulator::createCollisionPool(vector<Entity*> entities, ColliderProvider provider) {
	map<Collideable*, Entity*> pool;
	for (auto entity : entities) {
		auto collideables = provider(entity);
		if (collideables->size() > 0) {
			for (size_t i = 0; i < collideables->size(); i++) {
				pool.insert(std::pair<Collideable*, Entity*>(collideables->at(i), entity));
			}
		}
	}
	return pool;
}


vector<Entity*> Simulator::getCollideablesFromBroadPhase(vector<Entity*> entities) {
	auto debug = Configuration::getInstance()->getDebug();
	auto broad = vector<Entity*>();
	auto pool = this->createCollisionPool(entities, [](Entity* entity) -> vector<Collideable*>*{
		return entity->getBroadPhaseCollideables();
	});

	std::map<Collideable*, Entity*>::iterator x;
	std::map<Collideable*, Entity*>::iterator y;
	debug->broadPhase = false;
	for (x = pool.begin(); x != pool.end(); x++) {
		for (y = x; y != pool.end(); y++) {
			if (y != x && CollisionChecker().isColliding(x->first, y->first)) {
				debug->broadPhase = true;
				broad.push_back(x->second);
				broad.push_back(y->second);
			};
		}
	}

	return broad;
}

void Simulator::collideAtNarrowPhase(vector<Entity*> entities, float timestep) {
	auto debug = Configuration::getInstance()->getDebug();
	debug->narrowPhase = false;

	auto pool = this->createCollisionPool(entities, [](Entity* entity) -> vector<Collideable*>*{
		return entity->getNarrowPhaseCollideables();
	});;

	for (std::pair<Collideable*, Entity*> x : pool) {
		for (std::pair<Collideable*, Entity*> y : pool) {
			if (x != y) {
				auto restriction = CollisionChecker().isColliding(x.first, y.first);
				if (definitelyLessThan(restriction, 0.0f, 0.001f)) {
					debug->narrowPhase = true;
					auto allParticlesAffected = x.first->affectedParticles;
					for (size_t i = 0; allParticlesAffected != nullptr && i < allParticlesAffected->size(); i++) {
						// S� foi implementada a colis�o entre pontos e esferas
						auto particle = allParticlesAffected->at(i);
						auto point = dynamic_cast<CollideablePoint*>(x.first);
						auto sphere = dynamic_cast<CollideableBoudingSphere*>(y.first);
						auto vertices = x.second->getObject()->getVertices();
					
						auto A = sphere->getPosition();
						auto B = point->getPosition();
						auto N = glm::normalize(B - A);
						auto projection = sphere->getRadius() * N;
						auto contact = (sphere->getPosition() + projection) - abs(sphere->getRadius() - restriction);

						vec3 force = this->calculateForces(
							vertices->at(particle->i),
							contact,
							0.0f,
							0.002f
						);
						particle->force += force;
						vertices->at(particle->i) += integrate(particle, timestep, 0.9f);
					}
				};
			}
		}
	}
}

void Simulator::update(Scene* scene) {
	const auto timestep = Configuration::getInstance()->getSimulationParams()->step;
	auto entities = scene->getEntities();

	for (auto entity : entities) {
		if (!entity->getSimulationProperties().isStatic) {
			this->updateEntity(entity, timestep, 0.99f);
		}
	}

	auto broadPhase = this->getCollideablesFromBroadPhase(entities);
	this->collideAtNarrowPhase(broadPhase, timestep);
}

void Simulator::updateEntity(Entity* entity, const float timestep, const float damping) {
	auto particles = entity->getParticles();
	auto object = entity->getObject();
	auto vertices = object->getVertices();
	auto fixed = entity->getFixed();

	// update forces
	for (auto &spring: *entity->getSprings()) {
		auto p1 = &particles->at(spring.i);
		auto p2 = &particles->at(spring.j);

		vec3 force = this->calculateForces(
			vertices->at(p1->i), 
			vertices->at(p2->i), 
			spring.restLength, 
			entity->getSimulationProperties().stiffness
		);

		p1->force += force;
		p2->force -= force;
	}

	// time integration
	for (auto i = 0; i < particles->size(); i++) {
		if (std::find(fixed.begin(), fixed.end(), i) == fixed.end()) {
			auto particle = &particles->at(i);
			externalForces->apply(entity, timestep);
			vertices->at(particle->i) += integrate(particle, timestep, damping);
		}
	}
}

Simulator::Simulator() {
	this->externalForces = new ExternalForce();
}