#version 430 core

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_proj;

layout(location = 0) in vec3 position;
layout(location = 1) in vec4 color;

layout(location = 0) out vec4 fs_col;

void main() {
	fs_col = color;
	vec4 mPos = u_model * vec4(position, 1.f);
	gl_Position = u_proj * u_view * mPos;
}