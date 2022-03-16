#include "simulation.hpp"

void Simulator::update(Scene* scene) {
	const auto params = Configuration::getInstance()->getSimulationParams();
	const auto timestep = params->step;
	auto entities = scene->getEntities();

	for (auto entity : entities) {
		if (!entity->getSimulationProperties().isStatic) {
			this->updateEntity(entity, timestep, 0.99f);
		}
	}

	vector<Collideable*> pool;

	for (auto entity : entities) {
		// TODO: verificar se é colidivel
		auto collisions = entity->getCollideables();
		if (collisions->size() > 0) {
			pool.insert(std::end(pool), std::begin(*collisions), std::end(*collisions));
		}
	}

	const auto debug = Configuration::getInstance()->getDebug();
	debug->broadPhase = false;
	for (size_t i = 0; i < pool.size(); i++) {
		for (size_t j = i; j < pool.size(); j++) {
			if (i != j) {
				if (CollisionChecker().isColliding(pool.at(j), pool.at(i))) {
					debug->broadPhase = true;
				};
			}
		}
	}
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
		auto actualSpringVector = vertices->at(p1->i) - vertices->at(p2->i);
		auto actualLength = glm::length(actualSpringVector);
		auto distance = actualLength - spring.restLength;
		
		auto h = -entity->getSimulationProperties().stiffness * distance;
		auto force = actualSpringVector * h;

		p1->force += force;
		p2->force -= force;
	}

	// apply external forces

	// time integration
	for (auto i = 0; i < particles->size(); i++) {
		if (std::find(fixed.begin(), fixed.end(), i) == fixed.end()) {
			auto particle = &particles->at(i);

			externalForces->apply(entity, timestep);

			particle->velocity = particle->velocity + (particle->force / (particle->mass * timestep));
			particle->velocity *= damping;

			vertices->at(particle->i) += (particle->velocity * timestep);
			particle->force = vec3();
		}
	}
}

void Simulator::timeIntegration(Entity* entity, const float delta_t) {
	
}

Simulator::Simulator() {
	this->externalForces = new ExternalForce();
}