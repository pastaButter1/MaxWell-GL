#version 450 core

layout(location = 0) out vec4 r_couleur;

in vec3 f_pos;
in vec3 f_normale;
in vec2 f_uv;

uniform vec3 u_dirLumiere;
uniform vec3 u_couleur;
uniform vec3 u_posCam;

void main()
{
	vec3 dirVue = normalize(u_posCam - f_pos);
	vec3 normale = f_normale;

	float diffuse = dot(u_dirLumiere, normale);

	float speculaire = dot(dirVue, reflect(normale, u_dirLumiere));

	r_couleur = vec4(u_couleur.xyz  * (abs(diffuse) + abs(speculaire) * 0.1), 0.5f);

	r_couleur = vec4(f_uv, 0.0f, 0.5f);
}