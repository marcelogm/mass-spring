#include "entity.hpp"

Entity::Entity(Object object, vector<ShaderInfo> shaders, vector<int> fixed, vec4 color, mat4 model, SimulationPorpertiesProvider propertyProvider) {
	auto properties = propertyProvider();
	for (size_t i = 0; i < object.getVertices()->size(); i++) {
		object.getVertices()->at(i) = vec4(object.getVertices()->at(i), 1.f) * model;
		object.getEstimate()->at(i) = vec4(object.getEstimate()->at(i), 1.f) * model;
	}
	this->original = object;
	this->actual = object;
	this->fixedParticles = fixed;
	this->provider = propertyProvider;

	this->particles = new vector<Particle>(this->actual.getVertices()->size());
	this->springs = new vector<Spring>(this->actual.getTriangles()->size() * 3);

#pragma unroll
	for (size_t i = 0; i < this->actual.getVertices()->size(); i++) {
		this->particles->at(i) = { i, vec3(), vec3(), 1.0f };
	}

	const vector<vec3>* vertices = this->actual.getVertices();
	for (size_t i = 0; i < this->actual.getTriangles()->size(); i++) {
		const Triangle triangle = this->actual.getTriangles()->at(i);
		auto i1 = (triangle.vertices[0]).position;
		auto i2 = (triangle.vertices[1]).position;
		auto i3 = (triangle.vertices[2]).position;
		auto v1 = vertices->at(i1);
		auto v2 = vertices->at(i2);
		auto v3 = vertices->at(i3);

		this->springs->at((i * 3) + 0) = { i1, i2, glm::length(v1 - v2) };
		this->springs->at((i * 3) + 1) = { i2, i3, glm::length(v2 - v3) };
		this->springs->at((i * 3) + 2) = { i3, i1, glm::length(v3 - v1) };
	}

	this->color = color;
	this->model = model;
	this->normalBuffer = new vector<vec3>(this->actual.getTriangles()->size() * 3);
	this->vertexBuffer = new vector<vec3>(this->actual.getTriangles()->size() * 3);

	this->info = {};
	this->info.shader = LoadShaders(&shaders.front());

	glGenVertexArrays(1, &this->info.VAO);
	glGenBuffers(1, &this->info.positionVBO);
	glGenBuffers(1, &this->info.normalVBO);
	glBindVertexArray(this->info.VAO);
}


OpenGLObjectInformation Entity::getOpenGLInformation() {
	return this->info;
}

vector<Spring>* Entity::getSprings() {
	return this->springs;
}

vector<int> Entity::getFixed() {
	return this->fixedParticles;
}

mat4* Entity::getModel() {
	return &this->model;
}

Object* Entity::getObject() {
	return &this->actual;
}

vector<Particle>* Entity::getParticles() {
	return this->particles;
}

vec4* Entity::getColor() {
	return &this->color;
}

void Entity::update() {
	const vector<vec3>* vertices = this->actual.getVertices();
	const vector<vec3>* normals = this->actual.getNormals();
	#pragma unroll
	for (size_t i = 0; i < this->actual.getTriangles()->size(); i++) {
		const Triangle triangle = this->actual.getTriangles()->at(i);
		auto v1 = vertices->at(triangle.vertices[0].position);
		auto v2 = vertices->at(triangle.vertices[1].position);
		auto v3 = vertices->at(triangle.vertices[2].position);
		auto p = glm::cross(v2 - v1, v3 - v1);
		vertexBuffer->at((i * 3) + 0) = v1;
		vertexBuffer->at((i * 3) + 1) = v2;
		vertexBuffer->at((i * 3) + 2) = v3;
		normalBuffer->at((i * 3) + 0) = v1 + glm::normalize(p);
		normalBuffer->at((i * 3) + 1) = v2 + glm::normalize(p);
		normalBuffer->at((i * 3) + 2) = v3 + glm::normalize(p);
	}
}


vector<vec3>* Entity::getRenderedVertices() {
	return vertexBuffer;
}

vector<vec3>* Entity::getRenderedNormals() {
	return normalBuffer;
}

SimulationProperties Entity::getSimulationProperties() {
	return provider();
}
