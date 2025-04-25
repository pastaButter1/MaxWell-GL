#version 450 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in vec3 g_pos[];
in vec2 g_uv[];
in vec3 g_normale[];

out vec3 f_pos;
out vec3 f_normale;
out vec2 f_uv;
out vec3 f_normaleFlat;

void main()
{
	f_normaleFlat = normalize(cross(g_pos[1] - g_pos[0], g_pos[2] - g_pos[0]));

	for(int i = 0; i < 3; i++)
	{
		gl_Position = gl_in[i].gl_Position;
		f_pos = g_pos[i];
		f_uv = g_uv[i];
		f_normale = g_normale[i];
		EmitVertex();
	}

	EndPrimitive();
}