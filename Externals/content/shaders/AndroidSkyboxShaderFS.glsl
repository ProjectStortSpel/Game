varying vec3 TexCoord;
uniform samplerCube cubemap;

void main()
{
	gl_FragColor = textureCube(cubemap, TexCoord);
}