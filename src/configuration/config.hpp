#pragma once
#include <glm/glm.hpp>

using glm::vec4;

typedef struct {
	bool broadPhase;
	bool narrowPhase;
	bool showCollision;
	glm::vec3 debugPosition;
	float radiusModifier;
	float collisionStiffness;
} Debug;

typedef struct {
	float width;
	float height;
} ViewportDimension;

typedef struct {
	vec4 lightPosition;
	float ambientLightStrength;
	float diffuseLightStrength;
} LightConfiguration;

enum class RenderMode {
	TRIANGLES = 0,
	LINES = 1,
	POINTS = 2
};

typedef struct {
	RenderMode mode;
} OpenGLConfiguration;

typedef struct {
	float gravityModifier;
	float windModifier;
	glm::vec3 windDirection;
	int iterations;
	float stiffness;
	float bendability;
	float step;
	int substeps;
	bool llt;
	bool collision;
} SimulationParams;

class Configuration {
private:
	Configuration();
	static Configuration* instance;
	LightConfiguration light;
	ViewportDimension viewport;
	SimulationParams params;
	OpenGLConfiguration openGL;
	Debug debug;
public:
	static Configuration* getInstance();
	ViewportDimension* getViewport();
	LightConfiguration* getLight();
	SimulationParams* getSimulationParams();
	OpenGLConfiguration* getOpenGLConfiguration();
	Debug* getDebug();
};