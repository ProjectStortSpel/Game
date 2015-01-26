#version 100
#extension GL_OES_depth_texture : enable
#extension OES_fragment_precision_high : enable
#extension GL_EXT_frag_depth : enable

precision highp float;

varying vec2 TexCoord;

uniform sampler2D diffuseTex;

void main() {
	//Do not write depth if diffuse.a < 0.5
	if( texture2D( diffuseTex, TexCoord ).a < 0.5 )
		discard;

	///* Generate shadow map - write fragment depth. */
 //   float value = 10.0 - vTexCoord.z;
 //   float v = floor(value);
 //   float f = value - v;
 //   float vn = v * 0.1;
 //   gl_FragColor = vec4(vn, f, 0.0, 1.0);
	// gl_FragColor = vec4(gl_FragCoord.z, gl_FragCoord.z, gl_FragCoord.z, 1.0); 
}
