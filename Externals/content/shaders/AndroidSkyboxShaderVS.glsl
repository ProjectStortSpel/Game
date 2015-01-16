attribute vec3 VertexPoint;

varying vec3 TexCoord;

uniform mat4 MVP;

void main()
{
	gl_Position = MVP * vec4(VertexPoint, 1.0);
	TexCoord = VertexPoint;
}