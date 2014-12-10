#version 430 core

layout(points) in;
layout(triangle_strip, max_vertices = 4) out;

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

/*#version 430 core

layout(points) in;
layout(triangle_strip, max_vertices = 4) out;

out vec2 v_textureCoord;

void main() 
{
	// FRONT
	gl_Position = vec4( 10, -10, -10, 1 );
	v_textureCoord = vec2(0.0f, 0.665f);
	EmitVertex();
	gl_Position = vec4( 10,  10, -10, 1 );
	v_textureCoord = vec2(0.0f, 0.335f);
	EmitVertex();
	gl_Position = vec4(-10, -10, -10, 1 );
	v_textureCoord = vec2(0.25f, 0.665f);
	EmitVertex();
	gl_Position = vec4(-10,  10, -10, 1 );
	v_textureCoord = vec2(0.25f, 0.335f);
	EmitVertex();
	EndPrimitive(); 

	// BACK
	gl_Position = vec4(-10, -10,  10, 1 );
	v_textureCoord = vec2(0.5f, 0.665f);
	EmitVertex();
	gl_Position = vec4(-10,  10,  10, 1 );
	v_textureCoord = vec2(0.5f, 0.335f);
	EmitVertex();
	gl_Position = vec4( 10, -10,  10, 1 );
	v_textureCoord = vec2(0.75f, 0.665f);
	EmitVertex();
	gl_Position = vec4( 10,  10,  10, 1 );
	v_textureCoord = vec2(0.75f, 0.335f);
	EmitVertex();
	EndPrimitive(); 

	// RIGHT
	gl_Position = vec4( 10, -10,  10, 1 );
	v_textureCoord = vec2(0.75f, 0.665f);
	EmitVertex();
	gl_Position = vec4( 10,  10,  10, 1 );
	v_textureCoord = vec2(0.75f, 0.335f);
	EmitVertex();
	gl_Position = vec4( 10, -10, -10, 1 );
	v_textureCoord = vec2(1.0f, 0.665f);
	EmitVertex();
	gl_Position = vec4( 10,  10, -10, 1 );
	v_textureCoord = vec2(1.0f, 0.335f);
	EmitVertex();
	EndPrimitive(); 

	// LEFT
	gl_Position = vec4(-10, -10, -10, 1 );
	v_textureCoord = vec2(0.25f, 0.665f);
	EmitVertex();
	gl_Position = vec4(-10,  10, -10, 1 );
	v_textureCoord = vec2(0.25f, 0.335f);
	EmitVertex();
	gl_Position = vec4(-10, -10,  10, 1 );
	v_textureCoord = vec2(0.5f, 0.665f);
	EmitVertex();
	gl_Position = vec4(-10,  10,  10, 1 );
	v_textureCoord = vec2(0.5f, 0.335f);
	EmitVertex();
	EndPrimitive(); 

	// TOP
	gl_Position = vec4( 10,  10, -10, 1 );
	v_textureCoord = vec2(0.251f, 0.001f);
	EmitVertex();
	gl_Position = vec4( 10,  10,  10, 1 );
	v_textureCoord = vec2(0.499f, 0.001f);
	EmitVertex();
	gl_Position = vec4(-10,  10, -10, 1 );
	v_textureCoord = vec2(0.251f, 0.335f);
	EmitVertex();
	gl_Position = vec4(-10,  10,  10, 1 );
	v_textureCoord = vec2(0.499f, 0.335f);
	EmitVertex();
	EndPrimitive(); 

	// BOTTOM
	gl_Position = vec4( 10, -10,  10, 1 );
	v_textureCoord = vec2(0.251f, 0.667f);
	EmitVertex();
	gl_Position = vec4( 10, -10, -10, 1 );
	v_textureCoord = vec2(0.499f, 0.667f);
	EmitVertex();
	gl_Position = vec4(-10, -10,  10, 1 );
	v_textureCoord = vec2(0.251f, 0.999f);
	EmitVertex();
	gl_Position = vec4(-10, -10, -10, 1 );
	v_textureCoord = vec2(0.499f, 0.999f);
	EmitVertex();
	EndPrimitive(); 
}*/