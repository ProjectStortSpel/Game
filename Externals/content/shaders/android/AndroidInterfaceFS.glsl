#version 100
precision mediump float;

varying vec2 TexCoord;
uniform sampler2D diffuseTex;

void main() 
{
					// Diffuse tex
	gl_FragColor = texture2D( diffuseTex, TexCoord );
}