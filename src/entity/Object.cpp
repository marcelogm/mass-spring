#include "entity.hpp"

Object::Object() {
	this->vertices = vector<vec3>();
	this->normals = vector<vec3>();
	this->estimate = vector<vec3>();
	this->updateEdges();
}

Object::Object(vector<vec3> vertices, vector<vec3> normals, vector<Triangle> triangles) {
	this->vertices = vertices;
	this->estimate = vertices;
	this->normals = normals;
	this->triangles = triangles;
	this->updateEdges();
}

void Object::updateEdges() {
	this->edges = {};
	this->adjacents = {};
	for (size_t i = 0; i < triangles.size(); i++) {
		const Triangle triangle = triangles[i];

		const Edge first = { triangle.vertices[0], triangle.vertices[1] };
		const Edge second = { triangle.vertices[0], triangle.vertices[2] };
		const Edge third = { triangle.vertices[1], triangle.vertices[2] };

		this->edges.insert(first);
		this->edges.insert(second);
		this->edges.insert(third);

		this->adjacents[first].push_back(triangle);
		this->adjacents[second].push_back(triangle);
		this->adjacents[third].push_back(triangle);
	}
}

vector<vec3>* Object::getVertices() {
	return &vertices;
}

void Object::setVertices(vector<vec3> vertices) {
	this->vertices = vertices;
	this->updateEdges();
}

vector<vec3>* Object::getNormals() {
	return &normals;
}

vector<vec3>* Object::getEstimate() {
	return &estimate;
}

vector<Triangle>* Object::getTriangles() {
	return &triangles;
}

set<Edge, EdgeComparator>* Object::getEdges() {
	return &edges;
}

map<Edge, vector<Triangle>, EdgeComparator>* Object::getAdjacentTriangles() {
	return &adjacents;
}

size_t Object::getVerticesCount() {
	return triangles.size() * 3;
}
