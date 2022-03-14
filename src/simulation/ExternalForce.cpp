#include "simulation.hpp"

void ExternalForce::apply(Entity* entity, float delta_t) {
	auto particles = entity->getParticles();
	auto fixed = entity->getFixed();
	SimulationParams* params = Configuration::getInstance()->getSimulationParams();

	for (auto i = 0; i < particles->size(); i++) {
		if (std::find(fixed.begin(), fixed.end(), i) == fixed.end()) {
			auto particle = &particles->at(i);
			if (entity->getSimulationProperties().gravity) {
				particle->force = particle->force + vec3(0.f, (-9.81f / 1000000.f) * params->gravityModifier, 0.f) * particle->mass;
			}
			if (entity->getSimulationProperties().wind) {
				auto direction = params->windDirection;
				particle->force = particle->force + ((direction / 1000000.f) * params->windModifier) * particle->mass;
			}
		}
	}
}