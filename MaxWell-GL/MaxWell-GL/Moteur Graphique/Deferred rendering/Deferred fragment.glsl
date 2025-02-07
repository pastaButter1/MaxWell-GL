#version 450 core

layout(location = 0) out vec4 r_albedo;
layout(location = 1) out vec4 r_normal;
layout(location = 2) out vec4 r_material;

in vec2 f_uv;
in vec3 f_normal;
in vec3 f_pos;

uniform sampler2D u_albedo;

void main() 
{
	r_albedo = texture(u_albedo, f_uv);
	r_normal = vec4(f_normal, 1.0) * 0.5 + 0.5;
	r_material = vec4(1.0, 0.0, 1.0, 1.0);
}