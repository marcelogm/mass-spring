#include "entity.hpp"

Object ObjectProvider::get(string path) {
	auto vertices = vector<vec3>();
	auto normals = vector<vec3>();
	auto triangles = vector<Triangle>();

	FILE* file = fopen(path.c_str(), "r");
	if (file == NULL) {
		return Object();
	}
	
	while (true) {
		char line[128];
		int res = fscanf(file, "%s", line);
		if (res == EOF) break;

		if (strcmp(line, "v") == 0) {
			vec3 vertex;
			(void) fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			vertices.push_back(vertex);
		}
		else if (strcmp(line, "vn") == 0) {
			glm::vec3 normal;
			(void) fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			normals.push_back(normal);
		}
		else if (strcmp(line, "f") == 0) {
			unsigned int vertexIndex[3], normalIndex[3];
			(void) fscanf(file, "%d//%d %d//%d %d//%d\n",
				&vertexIndex[0], &normalIndex[0],
				&vertexIndex[1], &normalIndex[1], 
				&vertexIndex[2], &normalIndex[2]);
			Triangle triangle;
			for (size_t i = 0; i < 3; i++) {
				triangle.vertices[i] = { vertexIndex[i] - 1, normalIndex[i] - 1 };
			}
			triangles.push_back(triangle);
		}
	}

	return Object(vertices, normals, triangles, 1.0f);
}


