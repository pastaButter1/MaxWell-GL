#version 450 core

layout(location = 0) out vec4 a_couleur;

in vec2 f_uv;

void main()
{
	a_couleur = vec4(f_uv, 1, 0.5);
}