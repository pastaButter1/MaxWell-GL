#version 450 core

layout(location = 0) out vec4 r_couleur;

in vec3 f_pos;
in vec3 f_normale;
in vec2 f_uv;
in vec3 f_normaleFlat;

uniform vec3 u_dirLumiere;
uniform vec3 u_couleur;
uniform vec3 u_posCam;
uniform float u_disZ;
uniform float u_kd;
uniform float u_ks;
uniform float u_ka;
uniform float u_exposantSpec;
uniform samplerCube u_skybox;

void main()
{
	if(f_pos.x < u_disZ)
	{
		discard;
	}

	vec3 dirVue = normalize(f_pos - u_posCam);
	vec3 normale = (dot(dirVue, f_normaleFlat) > 0.0f) ? vec3(-1, 0, 0) : normalize(f_normale);

	float diffuse = dot(u_dirLumiere, normale) * u_kd;

	vec3 reflexion = reflect(dirVue, normale);
	float speculaire = max(diffuse > 0.0f ? max(dot(u_dirLumiere, reflexion) * u_ks, 0.0f) : 0.0f, 0.0f);
	speculaire = pow(speculaire, 32.0f * u_exposantSpec);

	vec3 reflexionSkybox = texture(u_skybox, reflexion).xyz;

	float ambient = u_ka;

	r_couleur = vec4(u_couleur.xyz  * (max(diffuse + speculaire, ambient)) + u_couleur * u_ks * reflexionSkybox, 1.0f);
}