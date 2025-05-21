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

    vec4 force =  champ / length(champ.xy);
    vec4 pos = coord * vec4(2.0f, 2.0f, 2.0f, 1.0f) - vec4(1.0f, 1.0f, 0.0f, 0.0f);

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