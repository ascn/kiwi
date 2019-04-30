#version 430 core

layout(binding = 0) uniform sampler2D u_shadowMap;

uniform vec3 u_lightPos;
uniform vec3 u_lightDir;

in VS_OUT {
	vec3 fs_pos;
	vec3 fs_nor;
	vec2 fs_uv;
	vec4 fs_lightSpacePos;
} fs_in;

out vec3 color;

float ShadowCalculation(vec4 lightSpacePos) {
	// perform perspective divide
	vec3 projCoords = lightSpacePos.xyz / lightSpacePos.w;
	// transform to [0,1] range
	projCoords = projCoords * 0.5 + 0.5;
	// get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
	float closestDepth = texture(u_shadowMap, projCoords.xy).r; 
	// get depth of current fragment from light's perspective
	float currentDepth = projCoords.z;
	
	vec3 lightDir = normalize(u_lightPos - fs_in.fs_pos);
	float bias = max(0.01 * (1.0 - dot(fs_in.fs_nor, lightDir)), 0.001);
	// check whether current frag pos is in shadow
	float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;
	if (projCoords.z > 1.0) { shadow = 0.0; }
	return shadow;
}

void main() {

	vec3 ambient = vec3(0.2, 0.2, 0.2);
	vec3 lightDir = normalize(u_lightPos - fs_in.fs_pos);
	vec3 diffuse = max(dot(lightDir, fs_in.fs_nor), 0.0) * vec3(0.5, 0.5, 0.5);
	
	float shadow = ShadowCalculation(fs_in.fs_lightSpacePos);

	color = ambient + (1. - shadow) * diffuse;
}