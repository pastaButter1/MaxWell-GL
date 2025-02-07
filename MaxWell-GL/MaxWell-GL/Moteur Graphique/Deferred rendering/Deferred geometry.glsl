#version 330 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in vec2 g_uv[3];
in vec3 g_normal[3];
in vec3 g_pos[3];

out vec2 f_uv;
out vec3 f_normal;
out vec3 f_pos;

uniform float u_faceOffset;
uniform mat4 u_cam;

void main() {
	float a = u_faceOffset;

	for(int i = 0; i < 3; i++) {
		gl_Position = u_cam * (gl_in[i].gl_Position + vec4(g_normal[i].xyz * a, 0)) ;
		f_uv = g_uv[i];
		f_normal = g_normal[i];
		f_pos = g_pos[i] + g_normal[i] * a;
		EmitVertex();
	}

	EndPrimitive();
}