#include "entity.hpp"

vector<ShaderInfo> DEFAULT_SHADER = {
	{ GL_VERTEX_SHADER, "resources/shaders/shader.vert" },
	{ GL_FRAGMENT_SHADER, "resources/shaders/shader.frag" },
	{ GL_NONE, NULL }
};

Entity::CollideableProvider NO_COLLISION = [](Entity* actual) -> vector<Collideable*>*{
	return new vector<Collideable*>();
};

Entity::SimulationPorpertiesProvider DEFAULT_PROPERTIES = []() -> SimulationProperties {
	return {
		false,
		false,
		1.0f,
		1.0f,
		1.0f,
		true
	};
};

Entity::CollideableProvider MIN_MAX_BOUDING_BOX = [](Entity* actual) -> vector<Collideable*>*{
	auto collideables = new vector<Collideable*>();
	collideables->push_back(new CollideableBoundingBox(
		[actual]() -> vec3 {
			auto vertices = actual->getObject()->getVertices();
			auto it = vertices->at(0);
#undef min
			for (size_t i = 1; i < vertices->size(); i++) {
				it = glm::min(it, vertices->at(i));
			}
			return it;
		},
		[actual]() -> vec3 {
			auto vertices = actual->getObject()->getVertices();
			auto it = vertices->at(0);
#undef max
			for (size_t i = 1; i < vertices->size(); i++) {
				it = glm::max(it, vertices->at(i));
			}
			return it;
		}
		));
	return collideables;
};

Entity::Entity(Object object, vector<ShaderInfo> shaders, vector<int> fixed, vec4 color, mat4 model, 
	SimulationPorpertiesProvider propertiesProvider, CollideableProvider broadCollideableProvider, CollideableProvider narrowCollideableProvider) {

	// Apply model
	for (size_t i = 0; i < object.getVertices()->size(); i++) {
		object.getVertices()->at(i) = model * vec4(object.getVertices()->at(i), 1.f);
		object.getEstimate()->at(i) = model * vec4(object.getEstimate()->at(i), 1.f);
	}

	// Buffers
	this->original = object;
	this->actual = object;
	this->fixedParticles = fixed;
	this->propertiesProvider = propertiesProvider;
	this->color = color;
	this->normalBuffer = new vector<vec3>(this->actual.getTriangles()->size() * 3);
	this->vertexBuffer = new vector<vec3>(this->actual.getTriangles()->size() * 3);
	this->info = {};
	this->info.shader = LoadShaders(&shaders.front());

	// Setup particles and spring
	this->particles = new vector<Particle>(this->actual.getVertices()->size());
	this->springs = new vector<Spring>(this->actual.getTriangles()->size() * 3);
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

	// Debug collision boxes
	this->broadPhaseCollideables = broadCollideableProvider(this);
	this->broadPhaseCollisionEntities = createCollisionEntities(this->broadPhaseCollideables);
	this->narrowPhaseCollideables = narrowCollideableProvider(this);
	this->narrowPhaseCollisionEntities = createCollisionEntities(this->narrowPhaseCollideables);

	glGenVertexArrays(1, &this->info.VAO);
	glGenBuffers(1, &this->info.positionVBO);
	glGenBuffers(1, &this->info.normalVBO);
	glBindVertexArray(this->info.VAO);
}

Entity::Entity(Object object, vec4 color, mat4 model) : Entity(object, DEFAULT_SHADER, {}, color, model, DEFAULT_PROPERTIES, NO_COLLISION, NO_COLLISION) {};

Entity::Entity(Object object, vec4 color) : Entity(object, color, mat4()) {};

OpenGLObjectInformation Entity::getOpenGLInformation() {
	return this->info;
}

vector<Spring>* Entity::getSprings() {
	return this->springs;
}

vector<int> Entity::getFixed() {
	return this->fixedParticles;
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

void Entity::updateModel(mat4 model) {
	for (size_t i = 0; i < actual.getVertices()->size(); i++) {
		actual.getVertices()->at(i) = model * vec4(original.getVertices()->at(i), 1.f);
		actual.getEstimate()->at(i) = model * vec4(original.getEstimate()->at(i), 1.f);
	}
	this->update();
}

void Entity::update() {
	for (size_t i = 0; i < this->broadPhaseCollideables->size(); i++) {
		broadPhaseCollideables->at(i)->update();
	}
	// TODO: optimize
	for (size_t i = 0; i < this->narrowPhaseCollideables->size(); i++) {
		narrowPhaseCollideables->at(i)->update();
	}
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
	return propertiesProvider();
}

vector<Entity>* Entity::getBroadPhaseCollisionEntities() {
	this->updateCollisionEntities(this->broadPhaseCollideables, this->broadPhaseCollisionEntities);
	return this->broadPhaseCollisionEntities;
}

vector<Entity>* Entity::getNarrowPhaseCollisionEntities() {
	this->updateCollisionEntities(this->narrowPhaseCollideables, this->narrowPhaseCollisionEntities);
	return this->narrowPhaseCollisionEntities;
}

vector<Collideable*>* Entity::getBroadPhaseCollideables() {
	return broadPhaseCollideables;
}

vector<Collideable*>* Entity::getNarrowPhaseCollideables() {
	return narrowPhaseCollideables;
}

// DRY IT
vector<Entity>* Entity::createCollisionEntities(vector<Collideable*>* collisions) {
	auto created = new vector<Entity>();
	for (size_t i = 0; i < collisions->size(); i++) {
		collisions->at(i)->update();
		if (collisions->at(i)->getType() == CollideableType::BOX) {
			auto box = dynamic_cast<CollideableBoundingBox*>(collisions->at(i));
			created->push_back(DebugEntityUtils::getDebugBoxEntityWith(box->getMin(), box->getMax()));
		}
		else if (collisions->at(i)->getType() == CollideableType::SPHERE) {
			auto sphere = dynamic_cast<CollideableBoudingSphere*>(collisions->at(i));
			created->push_back(DebugEntityUtils::getDebugBoxEntityWith(sphere->getPosition(), sphere->getRadius()));
		}
		else {
			auto sphere = dynamic_cast<CollideablePoint*>(collisions->at(i));
			created->push_back(DebugEntityUtils::getDebugBoxEntityWith(sphere->getPosition()));
		}
	}
	return created;
}

// DRY IT
void Entity::updateCollisionEntities(vector<Collideable*>* collideables, vector<Entity>* entities) {
	for (size_t i = 0; i < collideables->size(); i++) {
		collideables->at(i)->update();
		if (collideables->at(i)->getType() == CollideableType::BOX) {
			auto box = dynamic_cast<CollideableBoundingBox*>(collideables->at(i));
			entities->at(i).getObject()->setVertices(DebugEntityUtils::getDebugBoxVertices(box->getMin(), box->getMax()));
		}
		else if (collideables->at(i)->getType() == CollideableType::SPHERE) {
			auto sphere = dynamic_cast<CollideableBoudingSphere*>(collideables->at(i));
			entities->at(i).getObject()->setVertices(DebugEntityUtils::getDebugBoxVerticesFromSphere(sphere->getPosition(), sphere->getRadius()));
		}
		else {
			auto point = dynamic_cast<CollideablePoint*>(collideables->at(i));
			entities->at(i).getObject()->setVertices(DebugEntityUtils::getDebugBoxVerticesFromSphere(point->getPosition(), 0.01f));
		}
	}
}
