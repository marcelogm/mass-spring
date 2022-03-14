#include "scene.hpp"
#include "scene.hpp"

Scene* ClothSceneFactory::build() {
	ObjectProvider provider = ObjectProvider();
	vector<ShaderInfo> shaders = {
			{ GL_VERTEX_SHADER, "resources/shaders/shader.vert" },
			{ GL_FRAGMENT_SHADER, "resources/shaders/shader.frag" },
			{ GL_NONE, NULL }
	};
	auto f1 = vec3(-0.8f, 0.2f, 0.8f);
	auto f2 = vec3(-0.8f, 0.2f, -1.2f);
	auto staticObjectProvider = []() -> SimulationProperties {
		return {
			false,
			false,
			1.0f,
			1.0f,
			1.0f,
			true
		};
	};
	Configuration::getInstance()->getSimulationParams()->stiffness = 0.2f;
	Configuration::getInstance()->getSimulationParams()->step = 0.00001;

	Entity* b1 = getDebugBox(f1, 0.2, shaders, staticObjectProvider);
	Entity* b2 = getDebugBox(f2, 0.2, shaders, staticObjectProvider);
	Entity* cloth = new Entity(
		provider.get("resources/tecido.obj"),
		shaders,
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
		}
	);
	Entity* floor = new Entity(
		provider.get("resources/plane.obj"),
		shaders,
		vector<int>(),
		vec4(1.0f, 1.0f, 1.0f, 1.0f),
		glm::translate(mat4(1), vec3(0.0f, -10.0f, 0.0f)),
		staticObjectProvider
	);
	vector<Entity*> entities = { floor, cloth, b1, b2 };
	Scene* scene = new Scene(
		entities,
		new Camera(vec3(0.4f, -1.0f, 4.75f), vec3(-0.4f, 0.0f, -0.92), -113, 0)
	);
	return scene;
}

Entity* ClothSceneFactory::getDebugBox(vec3 position, float scale, vector<ShaderInfo> shaders, Entity::SimulationPorpertiesProvider propertiesProvider) {
	ObjectProvider objectProvider = ObjectProvider();
	return new Entity(
		objectProvider.get("resources/sphere.obj"),
		shaders,
		vector<int>(),
		vec4(0.f, 0.10f, 0.20f, 1.0f),
		glm::scale(glm::translate(mat4(1), position), vec3(scale)),
		propertiesProvider
	);
}
