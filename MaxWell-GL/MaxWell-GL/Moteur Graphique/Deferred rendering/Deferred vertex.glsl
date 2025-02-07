#version 450 core

layout(location = 0) in vec3 a_pos;
layout(location = 1) in vec2 a_uv;
layout(location = 2) in vec3 a_normal;

out vec2 g_uv;
out vec3 g_normal;
out vec3 g_pos;

void main()
{
	gl_Position = vec4(a_pos, 1.0);
	g_pos = a_pos;
	g_uv = a_uv;
	g_normal = a_normal;
}