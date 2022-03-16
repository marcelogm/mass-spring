#include "entity.hpp"

Entity DebugEntityUtils::getDebugBoxEntityWith(vec3 min, vec3 max) {
	ObjectProvider provider = ObjectProvider();
	auto obj = provider.get("resources/box.obj");
	obj.setVertices(DebugEntityUtils::getDebugBoxVertices(min, max));
	return Entity(obj, vec4(0.0f, 0.0f, 0.0f, 1.0f));
}

Entity DebugEntityUtils::getDebugBoxEntityWith(vec3 position, float radius) {
	ObjectProvider provider = ObjectProvider();
	auto obj = provider.get("resources/box.obj");
	obj.setVertices(DebugEntityUtils::getDebugBoxVerticesFromSphere(position, radius));
	return Entity(obj, vec4(0.0f, 0.0f, 0.0f, 1.0f));
}

vector<vec3> DebugEntityUtils::getDebugBoxVerticesFromSphere(vec3 position, float radius) {
	return DebugEntityUtils::getDebugBoxVertices(
		vec3(position.x - radius, position.y - radius, position.z - radius),
		vec3(position.x + radius, position.y + radius, position.z + radius)
	);
}

vector<vec3> DebugEntityUtils::getDebugBoxVertices(vec3 min, vec3 max) {
	return {
		{ max.x, max.y, min.z },
		{ max.x, min.y, min.z },
		{ max.x, max.y, max.z },
		{ max.x, min.y, max.z },
		{ min.x, max.y, min.z },
		{ min.x, min.y, min.z },
		{ min.x, max.y, max.z },
		{ min.x, min.y, max.z }
	};
}
