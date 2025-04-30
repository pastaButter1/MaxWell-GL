#shader vertex
#version 450 core

uniform ivec2 dimension;
uniform int count;

out vec2 coordonnee;

void main()
{
    vec2 pos = vec2(fract(float(gl_VertexID) / float(dimension.x)), floor(float(gl_VertexID) / float(dimension.x)) / float(dimension.y));
    gl_Position = vec4(vec2(pos) + vec2(1.0f) / vec2(dimension) / 2.0f, 0.0f, 1.0f);
}


#shader geometry
#version 450 core

layout (points) in;
layout (triangle_strip, max_vertices = 8) out;

out float intensite;

uniform mat4 transformation;
uniform sampler3D carte;

const float scalaire = 0.015;

void main() 
{    
    vec4 coord = vec4(gl_in[0].gl_Position.xyz, 1.0f);
    vec4 champ = texture(carte, vec4(transformation * coord).xyz);

    intensite = length(champ);

    vec4 force =  normalize(champ);
    vec4 pos = coord * vec4(vec3(2.0f), 1.0f) - vec4(1.0f, 1.0f, 0.0f, 0.0f);

    gl_Position = pos + force * scalaire; 
    EmitVertex();
    
    gl_Position = pos + vec4(-force.y, force.x, force.zw) * scalaire; 
    EmitVertex();
    
    gl_Position = pos + vec4(force.y, -force.x, force.zw) * scalaire; 
    EmitVertex();

    EndPrimitive();

    gl_Position = pos + vec4(-force.y, force.x, force.zw) / 3 * scalaire; 
    EmitVertex();

    gl_Position = pos + (-force + vec4(-force.y, force.x, force.zw) / 3) * scalaire;
    EmitVertex();

    gl_Position = pos + (-force + vec4(force.y, -force.x, force.zw) / 3) * scalaire;
    EmitVertex();

    gl_Position = pos + vec4(force.y, -force.x, force.zw) / 3 * scalaire; 
    EmitVertex();

    gl_Position = pos + vec4(-force.y, force.x, force.zw) / 3 * scalaire; 
    EmitVertex();
    
    EndPrimitive();
}  


#shader fragment
#version 450 core

uniform sampler1D gradient;

in float intensite;
out vec4 FragColor;

void main()
{
    FragColor = texture(gradient, (intensite) * 0.00015);
}