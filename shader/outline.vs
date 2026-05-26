#version 330

layout(location = 0) in vec3 vertexPosition;
layout(location = 2) in vec3 vertexNormal;

uniform mat4 mvp;
uniform float outlineThickness;

void main() {
    vec3 expanded = vertexPosition + normalize(vertexNormal) * outlineThickness;
    gl_Position = mvp * vec4(expanded, 1.0);
}
