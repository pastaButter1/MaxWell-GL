#version 450 core

layout(location = 0) in vec3 a_pos;
layout(location = 1) in vec2 a_uv;
layout(location = 2) in vec3 a_normale;

out vec3 g_pos;
out vec2 g_uv;
out vec3 g_normale;

uniform mat4 u_cam;

void main()
{
	gl_Position = u_cam * vec4(a_pos, 1.0f);
	g_pos = a_pos;
	g_uv = a_uv;
	g_normale = a_normale;
}