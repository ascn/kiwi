#version 430 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uv;

layout(location = 0) out vec2 fs_uv;

void main() {
	fs_uv = uv;
	gl_Position = vec4(position, 1.f);
}