#version 450 core

layout(early_fragment_tests) in;

layout(location = 0) out vec4 r_couleur;

in vec3 f_uv;

uniform samplerCube u_skybox;

void main()
{
	vec3 uv = f_uv;
	r_couleur = vec4(texture(u_skybox, uv));
	//r_couleur = vec4(uv * 0.5f + 0.5f, 1.0f);
	//r_couleur = vec4(cos(gl_PrimitiveID), sin(gl_PrimitiveID), 0, 1.0f);
}