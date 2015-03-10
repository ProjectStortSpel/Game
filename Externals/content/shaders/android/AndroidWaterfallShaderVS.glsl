#version 100

attribute vec3 VertexPosition;
attribute float VertexStartTime;

varying float Transp;

uniform float Time; // Simulation time
uniform float ParticleLifetime; // Particle lifespan
uniform float Size; // Spritesize factor

uniform mat4 ProjectionMatrix;
uniform mat4 ModelView;


void main() 
{
   mat4 MVP = ProjectionMatrix * ModelView;
	
	// The render pass --------------
	float age = Time - VertexStartTime;
	
	gl_Position = MVP * vec4(VertexPosition, 1.0);

	vec4 eyep = ModelView * vec4(VertexPosition, 1.0);

	float sizeFactor = 1.0;
	Transp = 0.0;
	if(Time >= VertexStartTime)
	{
		sizeFactor = 1.0+(age/ParticleLifetime)*0.75;
		if(age < 0.250)
			Transp = age/0.250 - age/ParticleLifetime;
		else
			Transp = 1.0 - age / ParticleLifetime;
	}

	gl_PointSize = 200.0 * Size * sizeFactor /-eyep.z;
	// ----------------------------
}