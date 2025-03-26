#version 450 core

layout(location = 0) in vec2 a_pos;

out vec2 f_uv;

void main()
{
	gl_Position = vec4(a_pos, 0, 1);
	f_uv = a_pos * 0.5 + 0.5;
}