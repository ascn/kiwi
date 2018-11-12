#version 430 core

uniform mat4 MVP;

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uv;

void main() {
	gl_Position = MVP * vec4(position, 1.f);
}