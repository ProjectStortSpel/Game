
varying vec3 Normal;
varying vec3 Tan;
varying vec3 BiTan;
varying vec2 TexCoord;
varying vec3 ViewPos;


//Input textures
uniform sampler2D diffuseTex;
//uniform sampler2D normalTex;
//uniform sampler2D specularTex;

//uniform mat4 ViewMatrix;

//struct DirectionalLight {
//	vec3 Direction; // Light position in world coords.
//	vec3 Intensity; // Diffuse intensity
//	vec3 Color;
//};

//struct Pointlight {
//	vec3 Position; // Light position in world coords.
//	vec3 Intensity; // Diffuse intensity
//	vec3 Color;
//	float Range;
//};

//struct MaterialInfo {
//	float Ks;
//	float Shininess;
//};
//MaterialInfo Material;


void main() 
{
	// Diffuse tex
	vec4 albedo_tex = texture2D( diffuseTex, TexCoord );

	//gl_FragColor = vec4(ambient + diffuse, 1.0) * albedo_tex + vec4(spec, 0.0f);
	gl_FragColor = vec4(0.0, 0.0, 1.0, 1.0)+albedo_tex-albedo_tex; //albedo_tex;
}