#version 400 core

layout( location = 0 ) in vec3 vPosition;
layout( location = 1 ) in vec3 vNormal;

out vec4 color;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec4 objectColor;
uniform vec4 lightPosition;
uniform float ambientLightStrength;
uniform float diffuseLightStrength;

vec4 getLightDirection(vec4 position, vec4 lightPosition) {
    return normalize(lightPosition - position);
}

vec4 getAmbientLight(float strength, vec4 color) {
    return strength * color;
}

vec4 getDiffuseLight(float strength, vec4 normal, vec4 direction, vec4 color) {
    return strength * max(dot(normal, direction), 0.0f) * color;
}

void
main() {
    vec4 normal = vec4(vNormal, 1.0f);
    vec4 position = vec4(vPosition, 1.0f);

    vec4 direction = getLightDirection(position, lightPosition);
    vec4 ambient = getAmbientLight(ambientLightStrength, vec4(1.0f, 1.0f, 1.0f, 1.0f));
    vec4 diffuse = getDiffuseLight(diffuseLightStrength, normal, direction, vec4(1.0f, 1.0f, 1.0f, 1.0f));

    color = (ambient + diffuse) * objectColor;
    gl_Position = projection * view * model * vec4(vPosition, 1.0f);
}