#version 450 core

layout(location = 0) in vec2 a_pos;
layout(location = 1) in vec2 a_uv;

out vec2 f_uv;

uniform vec2 u_uvOrigin;
uniform vec2 u_uvCorner;

void main()
{
	gl_Position = vec4(a_pos, 0.0, 1.0);
	f_uv = u_uvOrigin + a_uv * u_uvCorner;
}