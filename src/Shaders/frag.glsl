#version 430 core

uniform sampler2D diffuse;

layout(location = 0) in vec3 fs_pos;
layout(location = 1) in vec3 fs_nor;
layout(location = 2) in vec2 fs_uv;

out vec3 color;
void main() {
	//color = texture(diffuse, fs_uv).rgb;
	color = fs_nor;
}