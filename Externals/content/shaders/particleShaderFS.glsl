#version 400
uniform sampler2D ParticleTex;
in float Transp;
layout ( location = 0 ) out vec4 FragColor;

uniform vec3 BlendColor;

void main()
{
	FragColor = texture(ParticleTex, gl_PointCoord);

	if(BlendColor != vec3(0.0f))
	{
		float blendFactor = 0.94f;
		FragColor.xyz = (1.0f-blendFactor)*FragColor.xyz + blendFactor * BlendColor; 
	}
	FragColor.a *= Transp;
}