#pragma once

#include <vgl.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <map>
#include <set>
#include <string>
#include <vector>
#include <stdexcept>
#include <fstream>
#include <LoadShaders.h>
#include <algorithm>
#include "../camera/camera.hpp"
#include <functional>
#include "collision/collision.hpp"

using std::string;
using glm::vec3;
using glm::vec4;
using glm::mat4;
using glm::value_ptr;
using std::vector;
using std::set;
using std::map;
using std::ifstream;

typedef struct {
	size_t position;
	size_t normal;
} Vertex;

typedef struct {
	Vertex vertices[2];
} Edge;

typedef struct {
	Vertex vertices[3];
} Triangle;

typedef struct {
	bool operator() (const Edge& a, const Edge& b) const {
		const std::less<std::pair<int, int>> comparator;
		return comparator(
			std::minmax(a.vertices[0].position, a.vertices[1].position),
			std::minmax(b.vertices[0].position, b.vertices[1].position)
		);
	}
} EdgeComparator;

class Object {
private:
	vector<vec3> vertices;
	vector<vec3> normals;
	vector<vec3> estimate;
	vector<Triangle> triangles;
	set<Edge, EdgeComparator> edges;
	map<Edge, vector<Triangle>, EdgeComparator> adjacents;
	void updateEdges();
public:
	Object();
	Object(vector<vec3>, vector<vec3>, vector<Triangle>);
	vector<vec3>* getVertices();
	void setVertices(vector<vec3> vertices);
	vector<vec3>* getNormals();
	vector<vec3>* getEstimate();
	vector<Triangle>* getTriangles();
	set<Edge, EdgeComparator>* getEdges();
	map<Edge, vector<Triangle>, EdgeComparator>* getAdjacentTriangles();
	size_t getVerticesCount();
};

class ObjectProvider {
public:
	Object get(string path);
};

typedef struct {
	GLuint VAO;
	GLuint positionVBO;
	GLuint normalVBO;
	GLuint shader;
} OpenGLObjectInformation;

typedef struct SimulationProperties {
	bool gravity;
	bool wind;
	float mass;
	float stiffness;
	float damping;
	bool isStatic;
};


class Entity {
public:
	using SimulationPorpertiesProvider = std::function<SimulationProperties()>;
	using CollideableProvider = std::function<vector<Collideable*>*(Entity*)>;

	// TODO: oh god, what a mess, please refactor to a builder 
	Entity(Object, vector<ShaderInfo>, vector<int>, vec4, mat4,
		SimulationPorpertiesProvider, CollideableProvider, CollideableProvider);
	Entity(Object, vec4, mat4);
	Entity(Object, vec4);

	OpenGLObjectInformation getOpenGLInformation();
	Object* getObject();
	vector<Particle>* getParticles();
	vector<Spring>* getSprings();
	vector<int> getFixed();
	vec4* getColor();
	void update();
	void updateModel(mat4 model);
	vector<vec3>* getRenderedVertices();
	vector<vec3>* getRenderedNormals();
	SimulationProperties getSimulationProperties();

	vector<Entity>* getBroadPhaseCollisionEntities();
	vector<Collideable*>* getBroadPhaseCollideables();
	vector<Entity>* getNarrowPhaseCollisionEntities();
	vector<Collideable*>* getNarrowPhaseCollideables();
private:
	Object original;
	Object actual;
	vector<int> fixedParticles;
	vector<Particle>* particles;
	vector<Spring>* springs;
	vector<vec3>* vertexBuffer;
	vector<vec3>* normalBuffer;
	vec4 color;
	OpenGLObjectInformation info;
	SimulationPorpertiesProvider propertiesProvider;


	vector<Collideable*>* broadPhaseCollideables;
	vector<Entity>* broadPhaseCollisionEntities;
	vector<Collideable*>* narrowPhaseCollideables;
	vector<Entity>* narrowPhaseCollisionEntities;
};

class DebugEntityUtils {
public:
	static Entity getDebugBoxEntityWith(vec3 min, vec3 max);
	static Entity getDebugBoxEntityWith(vec3 pos, float radius);
	static Entity getDebugBoxEntityWith(vec3 pos);
	static vector<vec3> getDebugBoxVerticesFromSphere(vec3 pos, float radius);
	static vector<vec3> getDebugBoxVertices(vec3 min, vec3 max);
};

extern vector<ShaderInfo> DEFAULT_SHADER;
extern Entity::CollideableProvider NO_COLLISION;
extern Entity::SimulationPorpertiesProvider DEFAULT_PROPERTIES;
extern Entity::CollideableProvider MIN_MAX_BOUDING_BOX;