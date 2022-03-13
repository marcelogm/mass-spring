#version 400 core

layout( location = 0 ) in vec3 vPosition;
layout( location = 1 ) in vec3 vNormal;

out vec4 color;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec4 objectColor;

void
main() {
    vec4 normal = vec4(vNormal, 1.0f);
    vec4 position = vec4(vPosition, 1.0f);

    color = objectColor;
    gl_Position = projection * view * model * vec4(vPosition, 1.0f);
}