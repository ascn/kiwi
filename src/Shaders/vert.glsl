#version 430 core

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_proj;
uniform mat4 u_modelInvTr;

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uv;

layout(location = 0) out vec3 fs_pos;
layout(location = 1) out vec3 fs_nor;
layout(location = 2) out vec2 fs_uv;

void main() {
	fs_uv = uv;
	//fs_nor = vec3(normalize(vec4(u_modelInvTr * vec4(normal, 0.f))));
	fs_nor = normal;

	vec4 mPos = u_model * vec4(position, 1.f);
	fs_pos = vec3(mPos);

	gl_Position = u_proj * u_view * mPos;
}