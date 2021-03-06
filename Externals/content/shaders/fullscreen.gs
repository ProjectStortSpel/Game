#version 440 core

layout(points) in;
layout(triangle_strip, max_vertices = 4) out;

in vec3 herp[];

out vec2 v_textureCoord;

void main() 
{
    gl_Position = vec4( 1.0, 1.0, 0.5, 1.0 );
    v_textureCoord = vec2( 1.0, 1.0 );
    EmitVertex();

    gl_Position = vec4(-1.0, 1.0, 0.5, 1.0 );
    v_textureCoord = vec2( 0.0, 1.0 ); 
    EmitVertex();

    gl_Position = vec4( 1.0,-1.0, 0.5, 1.0 );
    v_textureCoord = vec2( 1.0, 0.0 ); 
    EmitVertex();

    gl_Position = vec4(-1.0,-1.0, 0.5, 1.0 );
    v_textureCoord = vec2( 0.0, 0.0 ); 
    EmitVertex();

    EndPrimitive(); 
}