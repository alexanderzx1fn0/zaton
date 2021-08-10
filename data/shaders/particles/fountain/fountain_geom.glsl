#version 330 core

layout( points ) in;
layout( triangle_strip, max_vertices = 4 ) out;

uniform mat4 ProjectionMatrix;

in vData
{
    float size;
    vec4 color;
}vertex[];


out vec2 TexCoord;
out vec4 vertex_color;


void main()
{
    mat4 m = ProjectionMatrix;

    gl_Position = m * (vec4(-vertex[0].size, -vertex[0].size,0.0,0.0) + gl_in[0].gl_Position);
    TexCoord = vec2(0.0,0.0);
	vertex_color = vertex[0].color;
    EmitVertex();

    gl_Position = m * (vec4(vertex[0].size ,-vertex[0].size,0.0,0.0) + gl_in[0].gl_Position);
    TexCoord = vec2(1.0,0.0);
	vertex_color = vertex[0].color;
    EmitVertex();

    gl_Position = m * (vec4(-vertex[0].size, vertex[0].size,0.0,0.0) + gl_in[0].gl_Position);
    TexCoord = vec2(0.0,1.0);
    vertex_color = vertex[0].color;
    EmitVertex();

    gl_Position = m * (vec4(vertex[0].size, vertex[0].size, 0.0,0.0) + gl_in[0].gl_Position);
    TexCoord = vec2(1.0,1.0);
	vertex_color = vertex[0].color;
    EmitVertex();

    EndPrimitive();
}
