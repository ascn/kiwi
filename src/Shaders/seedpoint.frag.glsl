#version 430 core

layout(location = 0) in vec4 fs_col;

out vec3 color;
void main() {
	color = vec3(fs_col);
}