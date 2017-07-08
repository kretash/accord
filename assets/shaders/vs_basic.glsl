#version 330

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;

out vec3 diffuse;
out vec3 frag_normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

const float C1            = 0.429043;
const float C2            = 0.511664;
const float C3            = 0.743125;
const float C4            = 0.886227;
const float C5            = 0.247708;
const float u_scaleFactor = 1.0f;
const vec3 u_L00          = vec3(0.79, 0.44, 0.54);
const vec3 u_L1m1         = vec3(0.39, 0.35, 0.60);
const vec3 u_L10          = vec3(-0.34, -0.18, -0.27);
const vec3 u_L11          = vec3(-0.29, -0.06, 0.01);
const vec3 u_L2m2         = vec3(-0.26, -0.22, -0.47);
const vec3 u_L2m1         = vec3(-0.11, -0.05, -0.12);
const vec3 u_L20          = vec3(-0.16, -0.09, -0.15);
const vec3 u_L21          = vec3(0.56, 0.21, 0.14);
const vec3 u_L22          = vec3(0.21, -0.05, -0.30);

vec3 spherical_harmonics(vec3 N)
{
	return
		C4 * u_L00 +
		2.0 * C2 * u_L11 * N.x +
		2.0 * C2 * u_L1m1 * N.y +
		2.0 * C2 * u_L10 * N.z +
		C1 * u_L22 * (N.x * N.x - N.y * N.y) +
		C3 * u_L20 * N.z * N.z - C5 * u_L20 +
		2.0 * C1 * u_L2m2 * N.x * N.y +
		2.0 * C1 * u_L21 * N.x * N.z +
		2.0 * C1 * u_L2m1 * N.y * N.z;
}

void main() {
	diffuse = spherical_harmonics(normal);
	frag_normal = normal.xyz * 0.5f + 0.5f;

	vec4 view_coords = view * model * vec4(position, 1.0f);
	gl_Position = projection * view_coords;
}