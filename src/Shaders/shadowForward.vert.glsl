#version 430 core

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_proj;
uniform mat4 u_modelInvTr;
uniform mat4 u_lightView;

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uv;

out VS_OUT {
	vec3 fs_pos;
	vec3 fs_nor;
	vec2 fs_uv;
	vec4 fs_lightSpacePos;
} vs_out;

void main() {
	vs_out.fs_uv = uv;
	vs_out.fs_nor = vec3(normalize(vec4(u_modelInvTr * vec4(normal, 0.f))));
	vec4 mPos = u_model * vec4(position, 1.f);
	vs_out.fs_pos = vec3(mPos);
	vs_out.fs_lightSpacePos = u_lightView * mPos;
	gl_Position = u_proj * u_view * mPos;
}