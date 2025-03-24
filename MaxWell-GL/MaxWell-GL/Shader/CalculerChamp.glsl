#shader compute
#version 450 core

layout(local_size_x = 10, local_size_y = 10, local_size_z = 1) in;

layout(rgba32f, binding = 0) uniform image3D positionVecteur;
layout(rgba32f, binding = 1) uniform image3D forceVecteur;
layout(r32f, binding = 2) uniform image3D champElectrique;
layout(rgba32f, binding = 3) uniform image1D origineFils;
layout(rgba32f, binding = 4) uniform image1D directionFils;

uniform int nombre_fils;

float carre(in float x)
{
	return x*x;
}

vec3 produitOrtogonal(in vec3 projete, in vec3 reference)
{
	return dot(projete, reference) / carre(length(reference)) * reference;
}

void main()
{
	ivec3 texelCoord = ivec3(gl_GlobalInvocationID.xyz);
	
	const float courant = 1000.0f;

	vec3 champ = vec3(0);

	bool est_fil = false;
	for (int i = 0; i < nombre_fils; i++)
	{
		vec3 position = imageLoad(positionVecteur, texelCoord).xyz;
		vec3 origine = imageLoad(origineFils, i).xyz;
		vec3 direction = imageLoad(directionFils, i).xyz;

		vec3 vecOP = origine - position; 
		vec3 vecDP = vecOP - produitOrtogonal(vecOP, direction);
		
		champ += 2.0f * courant * cross(direction, vecDP) / (length(vecDP) * length(vecDP));
	}

	imageStore(forceVecteur, texelCoord, vec4(champ, 0.0f)); 
}