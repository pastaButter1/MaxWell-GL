#version 450 core

layout (early_fragment_tests) in;

layout(location = 0) out vec4 r_color;

in vec2 f_uv;

uniform sampler2D u_depth;
uniform sampler2D u_albedo;
uniform sampler2D u_normal;
uniform sampler2D u_material;

uniform samplerCube u_cubemap;
uniform vec3 u_camPos;

uniform mat4 u_inverseProj;
uniform vec3 u_lightPos;

void main()
{
	float depth = texture(u_depth, f_uv).r * 2.0 - 1.0;
	vec4 screenPos = vec4(f_uv * 2.0 - 1.0, depth, 1.0);

	vec4 worldPos = u_inverseProj * screenPos;

	worldPos.xyz /= worldPos.w;

	vec3 viewDir = normalize(worldPos.xyz - u_camPos);

	vec3 normal = texture(u_normal, f_uv).xyz * 2.0 - 1.0;

	vec3 rayDir = reflect(normalize(worldPos.xyz - u_camPos), normal);

	r_color = (1.0 - texture(u_albedo, f_uv).r) * texture(u_cubemap, rayDir);

	//r_color = texture(u_normal, f_uv);
}