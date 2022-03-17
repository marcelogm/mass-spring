#include "scene.hpp"

Scene* ClothSceneFactory::build() {
	ObjectProvider provider = ObjectProvider();
	auto f1 = vec3(-0.8f, 0.2f, 0.8f);
	auto f2 = vec3(-0.8f, 0.2f, -1.2f);
	Configuration::getInstance()->getSimulationParams()->stiffness = 0.2f;
	Configuration::getInstance()->getSimulationParams()->step = 0.00001;
	Configuration::getInstance()->getSimulationParams()->step = 0.00001;

	Entity* b1 = getSphere(f1, 0.05);
	Entity* b2 = getSphere(f2, 0.05);
	Entity* cloth = new Entity(
		provider.get("resources/tecido.obj"),
		DEFAULT_SHADER,
		{ 0, 2 },
		vec4(0.39f, 0.83f, 0.71f, 1.0f),
		glm::scale(mat4(1), vec3(1.0f)),
		[]() -> SimulationProperties {
			return {
				true,
				true,
				1.0f,
				Configuration::getInstance()->getSimulationParams()->stiffness,
				0.99f,
				false
			};
		},
		MIN_MAX_BOUDING_BOX,
		[](Entity* actual) -> vector<Collideable*>* {
			auto collideables = new vector<Collideable*>();
			for (int i = 0; i < actual->getObject()->getVertices()->size(); i++) {
				collideables->push_back(new CollideablePoint(
					[actual, i]() -> vec3 {
						return actual->getObject()->getVertices()->at(i);
					},
					[actual, i]() -> vector<Particle*>* {
						auto v = new vector<Particle*>();
						v->push_back(&actual->getParticles()->at(i));
						return v;
					}
				));
			}
			return collideables;
		}
	);
	float radius = 0.5f;
	auto sphereCollision = [radius](Entity* actual) -> vector<Collideable*>*{
		auto collideables = new vector<Collideable*>();
		collideables->push_back(new CollideableBoudingSphere(
			[radius]() -> vec3 {
				return Configuration::getInstance()->getDebug()->debugPosition;
			}, radius + 0.2f)
		);
		return collideables;
	};
	Entity* sphere = new Entity(
		provider.get("resources/sphere.obj"),
		DEFAULT_SHADER,
		{},
		vec4(0.83f, 0.0f, 0.5f, 1.0f),
		mat4(1),
		[]() -> SimulationProperties {
			return {
				false,
				false,
				1.0f,
				0.0f,
				0.0f,
				true
			};
		},
		sphereCollision,
		sphereCollision
	);
	Entity* floor = new Entity(
		provider.get("resources/plane.obj"),
		vec4(1.0f, 1.0f, 1.0f, 1.0f),
		glm::translate(mat4(1), vec3(0.0f, -5.0f, 0.0f))
	);
	vector<Entity*> entities = { floor, cloth, b1, b2 };
	Scene* scene = new Scene(
		entities,
		sphere,
		[radius]() -> mat4 {
			return glm::scale(glm::translate(mat4(1), Configuration::getInstance()->getDebug()->debugPosition), vec3(radius));
		},
		new Camera(vec3(0.4f, -1.0f, 4.75f), vec3(-0.4f, 0.0f, -0.92), -113, 0)
	);
	return scene;
}

Entity* ClothSceneFactory::getSphere(vec3 position, float scale) {
	return new Entity(
		ObjectProvider().get("resources/sphere.obj"),
		vec4(0.f, 0.10f, 0.20f, 1.0f),
		glm::scale(glm::translate(mat4(1), position), vec3(scale))
	);
}
