#version 100
precision mediump float;

varying float Transp;

uniform sampler2D diffuseTex;
uniform vec3 BlendColor;

void main() 
{
	//Fix the color blending here
  // gl_FragColor = texture2D(diffuseTex, gl_PointCoord) * vColor;

   vec4 texColor = texture2D(diffuseTex, gl_PointCoord);
	if(texColor.a == 0.0)
		discard;

	gl_FragColor = texColor;

	if(BlendColor != vec3(0.0))
	{
		float blendFactor = 0.98;
		gl_FragColor.xyz = (1.0-blendFactor)*gl_FragColor.xyz + blendFactor * BlendColor; 
	}
	gl_FragColor.a *= Transp;
}