#version 430 core

uniform mat4 u_model;
uniform mat4 u_lightView;

layout(location = 0) in vec3 position;

void main() {
	gl_Position = u_lightView * u_model * vec4(position, 1.f);
}