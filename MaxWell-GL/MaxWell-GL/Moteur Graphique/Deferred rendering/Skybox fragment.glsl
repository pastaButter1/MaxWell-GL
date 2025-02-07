#version 450 core

layout (early_fragment_tests) in;

layout(location = 0) out vec4 r_color;

in vec3 f_dir;
in vec2 f_uv;

uniform samplerCube u_cubemap;
uniform vec3 u_camPos;
uniform mat4 u_inverseProj;

void main()
{
	float depth = 1;
	vec4 screenPos = vec4(f_uv * 2.0 - 1.0, depth, 1);

	vec4 worldPos = u_inverseProj * screenPos;

	worldPos.xyz /= worldPos.w;

	vec3 viewDir = normalize(u_camPos - worldPos.xyz);

	r_color = texture(u_cubemap, viewDir);
}