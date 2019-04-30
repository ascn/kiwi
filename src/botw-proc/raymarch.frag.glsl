#version 430 core

uniform vec2 u_resolution;
uniform float u_time;

layout(location = 0) in vec2 fs_uv;

out vec3 color;

#define M_PI 3.1415926535

// Functions for bounding volumes
struct Bounds {
	vec3 min;
	vec3 max;
};

struct Material {
	uint type;
	vec3 baseColor;
	vec3 specColor;
	float iorIn;
	float iorOut;
};

struct Intersection {
	vec3 point;
	float t;
	vec3 normal;
	Material material;
};

struct Light {
	vec3 pos;
	vec3 color;
	float intensity;
};

// Setup material types
const uint NULL_MATERIAL = 0;
const uint LAMBERT = 1;
const uint REFLECTIVE = 2;
const uint TRANSMISSIVE = 3;
const uint FRESNEL = 4;

// Setup materials
const Material BG_MATERIAL = {
	NULL_MATERIAL,
	vec3(0.f),
	vec3(0.f),
	1.f, 1.f
};

const Material LAMBERT_WHITE = {
	LAMBERT,
	vec3(0.95f),
	vec3(0.f),
	1.f, 1.f
};

const Material REFLECTIVE_WHITE = {
	REFLECTIVE,
	vec3(0.95f),
	vec3(0.95f),
	1.f, 1.f
};

const Material CLEAR_GLASS = {
	FRESNEL,
	vec3(0.95f),
	vec3(0.95f),
	1.f, 1.3f
};

/*
	// Light positions with intensity as w-component
	lights[0] = vec4(6.0, 3.0, 5.0, 2.0); // key light
	lights[1] = vec4(-6.0, 3.0, 5.0, 1.5); // fill light
	lights[2] = vec4(6.0, 5.0, -1.75, 4.0); // back light
	
	lightColor[0] = vec3(0.9, 0.5, 0.9);
	lightColor[1] = vec3(0.4, 0.7, 1.0);
	lightColor[2] = vec3(1.0, 1.0, .2);
*/

// Setup lights
const uint NUM_LIGHTS = 3;
const Light lights[NUM_LIGHTS] = {
	{ vec3(12.0, 6.0, 10.0), vec3(0.9, 0.5, 0.9), 2.0 },
	{ vec3(-12.0, 6.0, 10.0), vec3(0.4, 0.7, 1.0), 1.5 },
	{ vec3(12.0, 10.0, -3.5), vec3(1.0, 1.0, 0.2), 4.0 }
};

//Object setup
vec4 sph1 = vec4( 0.0, 0.0, 0.0, 1.0);

//=============================================================================
// BEGIN SDF FUNCTIONS
// From https://iquilezles.org/www/articles/distfunctions/distfunctions.htm
//=============================================================================

float opUnion(float d1, float d2) { return min(d1, d2); }
float opSubtraction(float d1, float d2) { return max(-d1, d2); }
float opIntersection(float d1, float d2) { return max(d1, d2); }

float opSmoothUnion(float d1, float d2, float k) {
	float h = clamp(0.5 + 0.5 * (d2 - d1) / k, 0.0, 1.0);
	return mix(d2, d1, h) - k * h * (1.0 - h);
}

float opSmoothSubtraction(float d1, float d2, float k) {
	float h = clamp(0.5 - 0.5 * (d2 + d1) / k, 0.0, 1.0);
	return mix(d2, -d1, h) - k * h * (1.0 - h);
}

float opSmoothIntersection(float d1, float d2, float k) {
	float h = clamp(0.5 - 0.5 * (d2 - d1) / k, 0.0, 1.0);
	return mix(d2, d1, h) + k * h * (1.0 - h);
}

float dot2(vec2 v1) {
	return dot(v1, v1);
}

//=============================================================================
// END SDF FUNCTIONS
//=============================================================================

//=============================================================================
// BEGIN GEOMETRY
//=============================================================================

float iSphere(in vec3 ro, in vec3 rd, in vec4 sph) {
	vec3 oc = ro - sph.xyz; //distance ray origin - sphere center
	
	float b = dot(oc, rd);
	float c = dot(oc, oc) - sph.w * sph.w; //sph.w is radius
	float h = b*b - c; //Commonly known as delta. The term a is 1 so is not included.
	
	float t;
	if(h < 0.0) 
		t = -1.0;
	else
		t = (-b - sqrt(h)); //Again a = 1.
	return t;
}

bool intersectSphere(in vec3 ro, in vec3 rd, in vec3 center, in float radius) {
	vec3 L = ro - center;
	float A = dot(rd, rd);
	float B = 2.f * dot(rd, L);
	float C = dot(L, L) - (radius * radius);
	float d = B * B - 4 * A * C;
	if (d < 0) { return false; }
	float t0, t1;
	t0 = (-B - sqrt(d)) / (2.f * A);
	t1 = (-B + sqrt(d)) / (2.f * A);
	if (t0 < 0 && t1 < 0) { return false; }
	return true;
}

// Sphere centered at center with radius r
float sdSphere(vec3 p, vec3 center, float radius) {
	return length(p - center) - radius;
}

vec3 nSphere(in vec3 pos, in vec3 center) {
	return normalize(pos - center);
}

float sdPlane(vec3 p, vec4 n) {
	return dot(p, n.xyz) + n.w;
}

float sdShrineSphere(vec3 p, out vec3 normal, out Material material) {
	float outerSphereT = sdSphere(p, vec3(0, 5, 0), 5);
	return outerSphereT;
}

float sdCappedCylinder( vec3 p, vec2 h )
{
  vec2 d = abs(vec2(length(p.xz),p.y)) - h;
  return min(max(d.x,d.y),0.0) + length(max(d,0.0));
}

float sdRoundCone( in vec3 p, in float r1, float r2, float h )
{
    vec2 q = vec2( length(p.xz), p.y );
    
    float b = (r1-r2)/h;
    float a = sqrt(1.0-b*b);
    float k = dot(q,vec2(-b,a));
    
    if( k < 0.0 ) return length(q) - r1;
    if( k > a*h ) return length(q-vec2(0.0,h)) - r2;
        
    return dot(q, vec2(a,b) ) - r1;
}

float sdCappedCone( in vec3 p, in float h, in float r1, in float r2 )
{
    vec2 q = vec2( length(p.xz), p.y );
    
    vec2 k1 = vec2(r2,h);
    vec2 k2 = vec2(r2-r1,2.0*h);
    vec2 ca = vec2(q.x-min(q.x,(q.y < 0.0)?r1:r2), abs(q.y)-h);
    vec2 cb = q - k1 + k2*clamp( dot(k1-q,k2)/dot2(k2), 0.0, 1.0 );
    float s = (cb.x < 0.0 && ca.y < 0.0) ? -1.0 : 1.0;
    return s*sqrt( min(dot2(ca),dot2(cb)) );
}

float sdShrine(vec3 p, out vec3 normal, out Material material) {
	normal = vec3(1, 0, 0);
	material = LAMBERT_WHITE;

	float c1 = sdCappedCone(p + vec3(0, -4, 0), 1, 1, 0.6);
	float c2 = sdCappedCone(p + vec3(0, -3.5, 0), 0.5, 2, 1);
	float c3 = sdCappedCone(p + vec3(0, -3, 0), 0.5, 3, 2);

	float c12 = opSmoothUnion(c1, c2, 1);
	float c123 = opSmoothUnion(c12, c3, 0.3);
	return c123;
}

float sdMap(in vec3 ro, in vec3 rd, vec3 p,
			out vec3 normal, out Material material) {
	float sphereT = 1e30;
	//if (intersectSphere(ro, rd, vec3(0, 1, 0), 0.5f)) {
		sphereT = sdSphere(p, vec3(0, 1, 0), 0.5f);
	//}
	//
	float shrineT = sdShrine(p, normal, material);
	sphereT = shrineT;

	vec4 planeNormal = vec4(0, 1, 0, 0.0);
	float planeT = sdPlane(p, planeNormal);
	if (sphereT > 0.f && sphereT < planeT) {
		normal = nSphere(p, sph1.xyz);
		material = LAMBERT_WHITE;
		return sphereT;
	} else if (planeT > 0.f && planeT < sphereT) {
		normal = planeNormal.xyz;
		material = LAMBERT_WHITE;
		return planeT;
	}
}

float sdMapShadow(vec3 p) {
	float sphereT = 1e30;
	sphereT = sdSphere(p, vec3(0, 1, 0), 0.5f);

	vec4 planeNormal = vec4(0, 1, 0, 0.0);
	float planeT = sdPlane(p, planeNormal);
	if (sphereT > 0.f && sphereT < planeT) {
		return sphereT;
	} else if (planeT > 0.f && planeT < sphereT) {
		return planeT;
	}
}

// t returns the distance to closest intersection
// return value is material id
Material intersect(in vec3 ro, in vec3 rd, out float resT, out vec3 resN) {
	resT = 1000.0;
	Material mat = BG_MATERIAL;
	float t = 0.0;
	for (int i = 0; i < 64; ++i) {
		vec3 p = ro + t*rd;
		vec3 normal = vec3(0);
		float tMap = sdMap(ro, rd, p, normal, mat);
		if (tMap < 0.01) {
			resT = t;
			resN = normal;
			return mat;
		}
		t += tMap;
		if (t > 25.f) {
			return BG_MATERIAL;
		}
	}
	return BG_MATERIAL;
}

//=============================================================================
// END GEOMETRY
//=============================================================================

//=============================================================================
// BEGIN SHADING
//=============================================================================

float shadow(in vec3 ro, in vec3 rd, in float k) {
	float t = 0.0;
	float res = 1.0;
	for (int i = 0; i < 64; ++i) {
		vec3 p = ro + t*rd;
		Material mat = BG_MATERIAL;
		vec3 normal = vec3(0.f);
		float tMap = sdMap(ro, rd, p, normal, mat);
		if (tMap < 0.005) {
			return 0.0;
		}
		res = min(res, k*tMap/t);
		t += tMap;
		if (t > 25.f) {
			break;
		}
	}
	return res;
}

// IQ's physically plausible shadow
float sphSoftShadow( in vec3 ro, in vec3 rd, in vec4 sph, in float k ) {
	vec3 oc = ro - sph.xyz;
	float b = dot( oc, rd );
	float c = dot( oc, oc ) - sph.w*sph.w;
	float h = b*b - c;
	

	// physically plausible shadow
	float d = sqrt( max(0.0,sph.w*sph.w-h)) - sph.w;
	float t = -b - sqrt( max(h,0.0) );
	return (t<0.0) ? 1.0 : smoothstep(0.0, 1.0, 2.5*k*d/t ); 
}

vec3 getColor(in vec3 pos, in vec3 nor, in vec3 rd, in Material material) {
	int currDepth = 5;

	vec3 col = vec3(0);

	vec3 curr_pos = pos;
	vec3 curr_nor = nor;
	vec3 curr_rd = rd;
	Material curr_mat = material;

	vec3 accumulatedColor = vec3(0);

	for (int i = currDepth; i > 0; --i) {
		if (curr_mat.type == LAMBERT) {
			col = curr_mat.baseColor;
			vec3 sum = vec3(0.0f);
			vec3 specSum = vec3(0.0f);
			
			for (int i = 0; i < 3; i++) { // Compute lambert for each light
				vec3 lambert = col *
					min(max(dot(curr_nor, normalize(lights[i].pos.xyz - pos)), 0.0f), 1.0f) *
					lights[i].intensity * lights[i].color;
				sum += lambert;
			}
			col = sum / 3.0; // Average resulting color
			break;
		} else if (curr_mat.type == REFLECTIVE) {
			curr_rd = reflect(curr_rd, curr_nor);
			float newT;
			curr_mat = intersect(curr_pos + 0.01*curr_nor, curr_rd, newT, curr_nor);
		} else if (curr_mat.type == TRANSMISSIVE) {

		} else if (curr_mat.type == FRESNEL) {

		} else if (curr_mat.type == NULL_MATERIAL) {
			col = vec3(0.f);
			break;
		}
	}

	// for each light calculate shadow
	float shadowTerm = 0.0;
	for (int i = 0; i < 3; ++i) {
		vec3 rd = normalize(lights[i].pos.xyz - pos);
		shadowTerm += shadow(pos + .05*nor, rd, 2.0f);
	}
	shadowTerm /= 3.0;

	return col * shadowTerm;
}

//=============================================================================
// END SHADING
//=============================================================================

void main() {
	//pixel coordinates from 0 to 1
	float aspectRatio = u_resolution.x/u_resolution.y;
	vec2 uv = gl_FragCoord.xy / u_resolution.xy;
	
	//generate a ray with origin ro and direction rd
	vec4 ro = vec4(0.0, 2.0, 5.0, 1.0);
	vec3 rd = normalize(vec3( (-1.0+2.0*uv) * vec2(aspectRatio, 1.0), -1.0));

	//intersect the ray with scene
	float t;
	vec3 n;
	Material mat = intersect(ro.xyz, rd, t, n);

	vec3 col = getColor(ro.xyz + t*rd, n, rd, mat);
	
	color = col;
}