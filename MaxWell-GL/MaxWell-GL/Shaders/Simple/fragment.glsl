#version 450 core

layout(location = 0) out vec4 r_couleur;

in vec3 f_pos;
in vec3 f_normale;
in vec2 f_uv;

uniform vec3 u_dirLumiere;
uniform vec3 u_couleur;
uniform vec3 u_posCam;
uniform float u_disZ;

void main()
{
	if(f_pos.x < u_disZ)
	{
		discard;
	}

	vec3 dirVue = normalize(u_posCam - f_pos);
	vec3 normale = (dot(dirVue, f_normale) < 0.0f) ? vec3(1, 0, 0) : f_normale;

	float diffuse = dot(u_dirLumiere, normale);

	float speculaire = dot(dirVue, reflect(normale, u_dirLumiere));

	r_couleur = vec4(u_couleur.xyz  * (diffuse + speculaire * 0.4), 1.0f);
}