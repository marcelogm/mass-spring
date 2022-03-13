#include "simulation.hpp"

void ExternalForce::apply(vector<Entity*>* entities, float delta_t) {
	const auto params = Configuration::getInstance()->getSimulationParams();

	for (size_t i = 0; i < entities->size(); i++) {
		// TODO: apply external forces
	}
}