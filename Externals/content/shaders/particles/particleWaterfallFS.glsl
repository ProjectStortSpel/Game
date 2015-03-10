#version 400
uniform sampler2D ParticleTex;
in float Transp;
layout ( location = 0 ) out vec4 FragColor;

uniform vec3 BlendColor;

void main()
{
	vec4 texColor = texture(ParticleTex, gl_PointCoord);
	if(texColor.a == 0.0f)
		discard;

	FragColor = texColor;

	if(BlendColor != vec3(0.0f))
	{
		float blendFactor = 0.74f;
		FragColor.xyz = (1.0f-blendFactor)*FragColor.xyz + blendFactor * BlendColor; 
	}
	FragColor.a *= Transp;
}