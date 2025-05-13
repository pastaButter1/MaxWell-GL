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
	float i;
	vec3 origine;
	float N;
	float L;
	float r;
	float R;
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
	
	return 2.0f * fil.courant * cross(direction, vecDP) / (length(vecDP) * length(vecDP));
}

vec3 champMagnetiqueSolenoide(ivec3 voxel, Solenoide solenoide)
{
	const float pi = 3.14159;
	const float mu0 = 4.0f*pi * 1.0e-6f;
	const vec3 coordonnee = imageLoad(voxelCoordonnee, voxel).xyz;
	const vec3 origine = solenoide.origine;
	const vec3 direction = solenoide.direction;
	const float i = solenoide.i;
	const float N = solenoide.N;
	const float L = solenoide.L;
	const float R = solenoide.R;

	vec3 integrale = vec3(0.0f);

	for (float teta = 0.0f; teta < solenoide.N * 2.0f*pi; teta += 0.1f)
	{
		vec3 dl = vec3(L/N, cos(teta), -sin(teta));
		vec3 u = vec3(teta*L / (2*pi*N), sin(teta)*R, cos(teta)*R) - (origine + coordonnee);
		float r = length(u);

		integrale += cross(dl,u) * pow(r, -3.0f);
	}

	return ((mu0*i) / (4.0f*pi)) * integrale;
};

void main()
{
	ivec3 voxel = ivec3(gl_GlobalInvocationID.xyz);
	
	vec3 champ = vec3(0);

	//for (int i = 0; i < nombreFils; i++)
	//{
	//	champ += champMagnetiqueFil(voxel, fils[i]);
	//}
	for (int i = 0; i < nombreSolenoides; i++)
	{
		champ += champMagnetiqueSolenoide(voxel, solenoides[i]);
	}

	imageStore(voxelChamp, voxel, vec4(champ * 1.e12f, 0.0f)); 
}