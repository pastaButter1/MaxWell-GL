#version 450 core

layout (vertices=4) out;

uniform uvec2 u_tailleCarte;
uniform mat4 u_cam;

out vec3 te_pos[];

void main()
{
    vec2 p[4] = 
    {
        vec2(-1, -1),
        vec2( 1, -1),
        vec2(-1,  1),
        vec2( 1,  1)
    };

    //gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;

    vec3 pos = vec3(p[gl_InvocationID].x, -1.0f, p[gl_InvocationID].y);

    gl_out[gl_InvocationID].gl_Position = u_cam * vec4(pos, 1.0f);

    te_pos[gl_InvocationID] = pos;

    if (gl_InvocationID == 0)
    {
        gl_TessLevelOuter[0] = u_tailleCarte.x;
        gl_TessLevelOuter[1] = u_tailleCarte.y;
        gl_TessLevelOuter[2] = u_tailleCarte.x;
        gl_TessLevelOuter[3] = u_tailleCarte.y;

        gl_TessLevelInner[0] = u_tailleCarte.x;
        gl_TessLevelInner[1] = u_tailleCarte.y;
    }
}