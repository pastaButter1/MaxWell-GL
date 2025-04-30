#version 450 core

struct Fil
{
	vec3 direction;
	float courant;
	vec3 origine;
};

struct Solenoide
{
	vec3 direction;
	float courant;
	vec3 origine;
	float nombreTours;
	float longeur;
};

layout(local_size_x = 10, local_size_y = 10, local_size_z = 1) in;

layout(rgba32f, binding = 0) uniform image3D voxelCoordonnee;
layout(rgba32f, binding = 1) uniform image3D voxelChamp;
layout(std430, binding = 2) readonly buffer BufferFils
{
	int nombreFils;
	Fil fils[];
};
layout(std430, binding = 3) readonly buffer BufferSolenoides
{
	int nombreSolenoides;
	Solenoide solenoides[];
};

float carre(in float x)
{
	return x*x;
}

vec3 produitOrtogonal(in vec3 projete, in vec3 reference)
{
	return dot(projete, reference) / carre(length(reference)) * reference;
}

vec3 champMagnetiqueFil(ivec3 voxel, Fil fil)
{
	vec3 coordonnee = imageLoad(voxelCoordonnee, voxel).xyz;
	vec3 origine = fil.origine.xyz;
	vec3 direction = fil.direction.xyz;

	vec3 vecOP = origine - coordonnee; 
	vec3 vecDP = vecOP - produitOrtogonal(vecOP, direction);
	
	return 2.0f * fil.courant * 1000.0f * cross(direction, vecDP) / (length(vecDP) * length(vecDP));
}

vec3 champMagnetiqueSolenoide(ivec3 voxel, Solenoide solenoide)
{
	const float u0 = 12.56636e-10;

	for (float teta = 0.0f; teta < solenoide.nombreTours * 6.28318f; teta += 0.25f)
	{
		
	}

	return vec3(0);
};

void main()
{
	ivec3 voxel = ivec3(gl_GlobalInvocationID.xyz);
	
	vec3 champ = vec3(0);

	for (int i = 0; i < nombreFils; i++)
	{
		champ += champMagnetiqueFil(voxel, fils[i]);
	}
	for (int i = 0; i < nombreSolenoides; i++)
	{
		champ += champMagnetiqueSolenoide(voxel, solenoides[i]);
	}

	imageStore(voxelChamp, voxel, vec4(champ, 0.0f)); 
}