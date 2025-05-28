#version 450 core

layout(location = 0) out vec4 couleur;

in vec3 f_pos;
in vec2 f_uv;
in vec4 f_couleur;
in vec3 f_normale;

uniform vec3 u_posCamera;

void main() 
{
	float a, b;
	a = modf(f_couleur.x * 20.0f, b);

	vec3 rayonCamera = normalize(u_posCamera - f_pos);
	vec3 dirLumiere = vec3(0, -1, 0);
	vec3 rayonLumiere = reflect(dirLumiere, f_normale);
	float diffuse = max(dot(rayonCamera, rayonLumiere), 0.0f);

	couleur = vec4(vec3(0.8f, 0.7f, 0.7f) * min(diffuse + 0.15f, 1.0f), 1.0f);
	
	//couleur = vec4(f_pos, 1.0f);
}