#version 450 core

layout(location = 0) in vec3 a_pos;

uniform mat4 u_cam;

void main()
{
	gl_Position = u_cam * vec4(a_pos, 1.0f);
}