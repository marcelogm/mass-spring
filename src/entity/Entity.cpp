#include "entity.hpp"

Entity::Entity(Object object, vector<ShaderInfo> shaders, vec4 color, mat4 model, bool gravity, float mass, bool isStatic) {
	for (size_t i = 0; i < object.getVertices()->size(); i++) {
		object.getVertices()->at(i) = vec4(object.getVertices()->at(i), 1.f) * model;
		object.getEstimate()->at(i) = vec4(object.getEstimate()->at(i), 1.f) * model;
	}
	this->isStatic = isStatic;
	this->original = object;
	this->actual = object;
	this->color = color;
	this->model = model;
	this->gravity = gravity;
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

mat4* Entity::getModel() {
	return &this->model;
}

Object* Entity::getObject() {
	return &this->actual;
}

vec4* Entity::getColor() {
	return &this->color;
}

bool Entity::isAffectedByGravity() {
	return gravity;
}

bool Entity::hasCollision() {
	return this->isStatic;
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
