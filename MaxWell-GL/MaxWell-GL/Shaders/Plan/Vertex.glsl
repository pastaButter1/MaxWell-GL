#version 450 core

uniform ivec2 dimension;

out vec2 coordonnee;

void main()
{
    vec2 pos = vec2(fract(float(gl_VertexID) / float(dimension.x)), floor(float(gl_VertexID) / float(dimension.x)) / float(dimension.y));
    gl_Position = vec4(vec2(pos) + vec2(1.0f) / vec2(dimension) / 2.0f, 0.0f, 1.0f);
}