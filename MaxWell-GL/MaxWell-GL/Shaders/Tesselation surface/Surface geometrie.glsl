#version 450 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in vec3 g_pos[];
in vec2 g_uv[];
in vec4 g_couleur[];

out vec3 f_pos;
out vec2 f_uv;
out vec4 f_couleur;
out vec3 f_normale;

void main() {

	f_normale = normalize(cross(g_pos[1] - g_pos[0], g_pos[2] - g_pos[1]));

	for(int i = 0; i < 3; i++)
	{
		gl_Position = gl_in[i].gl_Position;
		f_pos = g_pos[i];
		f_uv = g_uv[i];
		f_couleur = g_couleur[i];
		EmitVertex();
	}

	EndPrimitive();
}