#version 450

layout(local_size_x = 10, local_size_y = 10, local_size_z = 1) in;

layout(rgba32f, binding = 0) uniform image3D voxel;

uniform vec3 min;
uniform vec3 max;
uniform ivec3 dimension;

void main()
{
	ivec3 coord = ivec3(gl_GlobalInvocationID.xyz);
	vec3 scalaire = (max - min) / vec3(dimension);
	vec3 valeur = min + scalaire * (vec3(coord) + 0.5f);

	imageStore(voxel, coord, vec4(valeur, 0.0f));
}