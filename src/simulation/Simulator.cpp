#include "simulation.hpp"

void Simulator::update(Scene* scene) {
	const auto params = Configuration::getInstance()->getSimulationParams();
	const auto iterations = params->iterations;
	const auto delta_t = params->step / params->substeps;
	auto entities = scene->getEntities();

	this->externalForces->apply(&scene->getEntities(), delta_t);

	// integrate
	for (size_t step = 0; step < params->substeps; step++) {
		auto entities = scene->getEntities();
		for (int i = 0; i < entities.size(); i++) {
			this->timeIntegration(entities.at(i), delta_t);
		}
	}
}

void Simulator::timeIntegration(Entity* entity, const float delta_t) {
	
}

Simulator::Simulator() {
	this->externalForces = new ExternalForce();
}