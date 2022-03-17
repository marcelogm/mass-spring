#include "scene.hpp"

Scene* ClothSceneFactory::build() {
	ObjectProvider provider = ObjectProvider();

	Configuration::getInstance()->getSimulationParams()->stiffness = 0.2f;
	Configuration::getInstance()->getSimulationParams()->step = 0.01;
	Configuration::getInstance()->getSimulationParams()->windModifier = 0.0001;
	Configuration::getInstance()->getSimulationParams()->collision = true;
	Configuration::getInstance()->getDebug()->collisionStiffness = 1.0f;
	Configuration::getInstance()->getDebug()->debugPosition = glm::vec3(0.5f, -1.0f, -0.25f);

	// FIXED POINTS
	auto f1 = vec3(-0.8f, 0.2f, 0.8f);
	auto f2 = vec3(-0.8f, 0.2f, -1.2f);
	Entity* b1 = getSphere(f1, 0.05);
	Entity* b2 = getSphere(f2, 0.05);
	vector<int> fixedIndexes = { 0, 2  };

	// CLOTH

	auto r = ((double)rand() / (RAND_MAX)) + 0.6;
	auto g = ((double)rand() / (RAND_MAX)) + 0.6;
	auto b = ((double)rand() / (RAND_MAX)) + 0.6;
	Entity* cloth = new Entity(
		provider.get("resources/tecido.obj"),
		DEFAULT_SHADER,
		fixedIndexes,
		vec4(r, g, b, 1.0f),
		glm::scale(mat4(1), vec3(1.0f)),
		[]() -> SimulationProperties {
			auto stiffness = Configuration::getInstance()->getSimulationParams()->stiffness;
			return { true, true, 1.0f, stiffness, 0.99f, false };
		},
		MIN_MAX_BOUDING_BOX,
			[](Entity* actual) -> vector<Collideable*>*{
			auto collideables = new vector<Collideable*>();
			for (int i = 0; i < actual->getObject()->getVertices()->size(); i++) {
				collideables->push_back(new CollideablePoint(
					[actual, i]() -> vec3 {
						return actual->getObject()->getVertices()->at(i);
					},
					[actual, i]() -> vector<Particle*>*{
						auto v = new vector<Particle*>();
						v->push_back(&actual->getParticles()->at(i));
						return v;
					}
					));
			}
			return collideables;
		}
		);

	// SPHERE
	float radius = 0.5f;
	auto sphereCollision = [radius](Entity* actual) -> vector<Collideable*>*{
		auto collideables = new vector<Collideable*>();
		collideables->push_back(new CollideableBoudingSphere(
			[radius]() -> vec3 {
				return Configuration::getInstance()->getDebug()->debugPosition;
			}, [radius]() -> float {
				return radius * Configuration::getInstance()->getDebug()->radiusModifier;
			})
		);
		return collideables;
	};
	Entity* sphere = new Entity(
		provider.get("resources/sphere.obj"),
		DEFAULT_SHADER,
		{},
		vec4(0.f, 0.0f, 0.5f, 1.0f),
		mat4(1),
		[]() -> SimulationProperties {
			return { false, false, 1.0f, 0.0f, 0.0f, true };
		},
		sphereCollision,
		sphereCollision
	);

	// FLOOR
	Entity* floor = new Entity(
		provider.get("resources/plane.obj"),
		vec4(1.0f, 1.0f, 1.0f, 1.0f),
		glm::translate(mat4(1), vec3(0.0f, -5.0f, 0.0f))
	);

	// BUILD SCENE
	vector<Entity*> entities = { floor, b1, b2, cloth };
	return new Scene(
		entities,
		sphere,
		[radius]() -> mat4 {
			auto debugPosition = Configuration::getInstance()->getDebug()->debugPosition;
			return glm::scale(glm::translate(mat4(1), debugPosition), vec3(radius));
		},
		new Camera(vec3(0.4f, -1.0f, 4.75f), vec3(-0.4f, 0.0f, -0.92), -113, 0)
	);
}

Entity* ClothSceneFactory::getSphere(vec3 position, float scale) {
	return new Entity(
		ObjectProvider().get("resources/sphere.obj"),
		vec4(0.f, 0.10f, 0.20f, 1.0f),
		glm::scale(glm::translate(mat4(1), position), vec3(scale))
	);
}
