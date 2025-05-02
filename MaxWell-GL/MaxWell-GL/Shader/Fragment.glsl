#version 450 core

layout(location = 0) out vec4 FragColor;

uniform sampler1D gradient;

in float intensite;

void main()
{
    FragColor = texture(gradient, (intensite) * 0.00015);
    //FragColor = vec4(1,1,1,1);
}