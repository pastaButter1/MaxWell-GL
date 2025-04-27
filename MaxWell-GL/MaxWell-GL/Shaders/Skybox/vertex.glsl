#version 450 core

layout(location = 0) in vec3 a_pos;

out vec3 f_uv;

uniform mat4 u_cam;

void main()
{
	gl_Position = u_cam * vec4(a_pos, 1.0f);
	gl_Position.z = gl_Position.w;
	f_uv = a_pos;
}