#version 450 core

layout(location = 0) in vec2 a_pos;
layout(location = 1) in vec2 a_uv;

out vec3 f_dir;
out vec2 f_uv;

uniform vec2 u_rayDirBorder;
uniform mat3 u_camRot;

void main()
{
	gl_Position = vec4(a_pos, 0.0, 1.0);

	f_dir = vec3((a_pos * u_rayDirBorder), 1) * u_camRot;

	f_uv = a_uv;
}