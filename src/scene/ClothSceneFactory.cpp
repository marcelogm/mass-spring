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

	Configuration::getInstance()->getSimulationParams()->bendability = 0.05;
	Configuration::getInstance()->getSimulationParams()->stiffness = 0.20;
	Configuration::getInstance()->getSimulationParams()->iterations = 5;
	Configuration::getInstance()->getSimulationParams()->step = 0.01;
	Configuration::getInstance()->getSimulationParams()->substeps = 5;

	Entity* b1 = getDebugBox(f1, 0.2, shaders);
	Entity* b2 = getDebugBox(f2, 0.2, shaders);
	Entity* cloth = new Entity(
		provider.get("resources/tecido.obj"),
		shaders,
		vec4(0.39f, 0.83f, 0.71f, 1.0f),
		glm::scale(mat4(1), vec3(1.0f)),
		true,
		1.0f,
		false
	);
	Entity* floor = new Entity(
		provider.get("resources/plane.obj"),
		shaders,
		vec4(1.0f, 1.0f, 1.0f, 1.0f),
		glm::translate(mat4(1), vec3(0.0f, -10.0f, 0.0f)),
		false,
		0.0f,
		true
	);
	vector<Entity*> entities = { floor, cloth, b1, b2 };
	Scene* scene = new Scene(
		entities,
		new Camera(vec3(0.4f, -1.0f, 4.75f), vec3(-0.4f, 0.0f, -0.92), -113, 0)
	);
	return scene;
}

Entity* ClothSceneFactory::getDebugBox(vec3 position, float scale, vector<ShaderInfo> shaders) {
	ObjectProvider provider = ObjectProvider();
	return new Entity(
		provider.get("resources/sphere.obj"),
		shaders,
		vec4(0.f, 0.10f, 0.20f, 1.0f),
		glm::scale(glm::translate(mat4(1), position), vec3(scale)),
		false,
		1.0f,
		false
	);
}
