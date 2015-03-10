#version 400
subroutine void RenderPassType();
subroutine uniform RenderPassType RenderPass;
layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexVelocity;
layout (location = 2) in float VertexStartTime;
layout (location = 3) in vec3 VertexInitialVelocity;
layout (location = 4) in vec3 VertexInitialPosition;

out vec3 Position; // To transform feedback
out vec3 Velocity; // To transform feedback
out float StartTime; // To transform feedback
out float Transp; // To fragment shader

uniform float Time; // Simulation time
uniform float DeltaTime; // Elapsed time between frames dt
uniform vec3 Accel; // Particle acceleration
uniform float ParticleLifetime; // Particle lifespan

uniform float Size; // Spritesize factor
uniform int EndPhase;

uniform mat4 ProjectionMatrix;
uniform mat4 ModelView;

mat4 MVP;

subroutine (RenderPassType)
void update() 
{
	Position = VertexPosition;
	Velocity = VertexVelocity;
	StartTime = VertexStartTime;

	if( Time >= StartTime ) {
		float age = Time - StartTime;
		
		if( age >ParticleLifetime && EndPhase == 0) {
			// The particle is past its lifetime, recycle.
			Position = VertexInitialPosition; //vec3(0.0);
			Velocity = VertexInitialVelocity;
			StartTime = Time-(age-ParticleLifetime);
		} 
		else {
			// The particle is alive, update.
			Position += Velocity * DeltaTime;
			Velocity += Accel * DeltaTime;			
		}
	}
}
subroutine (RenderPassType)
void render() 
{
	float age = Time - VertexStartTime;
	
	gl_Position = MVP * vec4(VertexPosition, 1.0);

	vec4 eyep = ModelView * vec4(VertexPosition, 1.0);

	float sizeFactor = 1.0;
	Transp = 0.0;
	if(Time >= VertexStartTime)
	{
		sizeFactor = 1.0+(age/ParticleLifetime)*0.75;
		if(age < 0.250)
			Transp = age/0.250f - age/ParticleLifetime;
		else
			Transp = 1.0 - age / ParticleLifetime;
	}

	gl_PointSize = 200 * Size * sizeFactor /-eyep.z;
}

void main()
{
	MVP = ProjectionMatrix * ModelView;
	// This will call either render() or update()
	RenderPass();
}