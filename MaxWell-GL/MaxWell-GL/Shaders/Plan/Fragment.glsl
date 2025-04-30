#version 450 core

layout(location = 0) out vec4 FragColor;

uniform sampler1D gradient;

in float intensite;

void main()
{
    FragColor = vec4(texture(gradient, (intensite) * 0.00003).xyz, 1.0f);
}