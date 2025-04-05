#version 450 core

struct Fil
{
	vec3 direction;
	float courant;
	vec3 origine;
};

layout(local_size_x = 10, local_size_y = 10, local_size_z = 1) in;

layout(rgba32f, binding = 0) uniform image3D voxelCoordonnee;
layout(rgba32f, binding = 1) uniform image3D voxelChamp;
layout(std430, binding = 2) readonly buffer Buffer
{
	Fil fils[];
};

uniform int nombreFils;

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
	ivec3 voxel = ivec3(gl_GlobalInvocationID.xyz);
	
	const float courant = 1000.0f;

	vec3 champ = vec3(0);

	for (int i = 0; i < nombreFils; i++)
	{
		vec3 coordonnee = imageLoad(voxelCoordonnee, voxel).xyz;
		vec3 origine = fils[i].origine.xyz;
		vec3 direction = fils[i].direction.xyz;

		vec3 vecOP = origine - coordonnee; 
		vec3 vecDP = vecOP - produitOrtogonal(vecOP, direction);
		
		champ += 2.0f * courant * cross(direction, vecDP) / (length(vecDP) * length(vecDP));
	}

	imageStore(voxelChamp, voxel, vec4(champ, 0.0f)); 
}