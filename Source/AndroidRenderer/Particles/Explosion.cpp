#include "Explosion.h"

using namespace Renderer;

Explosion::Explosion()
{
}

Explosion::Explosion(const vec3 _pos, const vec3 _vel, int _nParticles, float _lifeTime, vec3 _scale, float _spriteSize, GLuint _texHandle, vec3 _color, Shader *_shaderProg) : ParticleEffect(_pos, _vel, _nParticles, _lifeTime, _scale, _spriteSize, _texHandle, _color, _shaderProg)
{
	//----Init starts here------------------
	//..-----Smoke-----..
	// Create and allocate buffers A and B for m_posBuf, m_velBuf and m_startTime
	m_dstBlendFactor = GL_ONE_MINUS_SRC_ALPHA;
	m_accel = vec3(0.0f, -0.8f, 0.0f);
	vec3 v(0.0f);
	float velocity, theta, phi;

	m_posData = new GLfloat[m_nrParticles * 3];
	m_velData = new GLfloat[m_nrParticles * 3];
	m_timeData = new GLfloat[m_nrParticles];

	m_particles = new Particle[m_nrParticles];

	srand(time(NULL));
	for (GLuint i = 0; i < m_nrParticles; i++) {
		vec3 pos = vec3(((float)(rand() % 9) - 4)*0.5f * m_scale.x,
						((float)(rand() % 9) - 4)*0.5f * m_scale.y,
						((float)(rand() % 9) - 4)*0.5f * m_scale.z
						);

		m_posData[3 * i] = pos.x;
		m_posData[3 * i + 1] = pos.y;
		m_posData[3 * i + 2] = pos.z;

		// Pick the direction of the velocity
		theta = glm::mix(0.0f, (float)M_PI / 6.0f, (float)(rand() % 101) / 100);
		phi = glm::mix(0.0f, (float)(2 * M_PI), (float)(rand() % 101) / 100);

		v.x = sin((float)(rand() % (int)(2 * M_PI * 1000) + 1) / 1000);  
		v.y = sin((float)(rand() % (int)(2 * M_PI * 1000) + 1) / 1000);  
		v.z = sin((float)(rand() % (int)(2 * M_PI * 1000) + 1) / 1000);  

		v = glm::normalize(v) * (((float)(rand() % 1001) + 300.f) / 500.f);

		// Scale to set the magnitude of the velocity (speed)
		velocity = glm::mix(1.25f, 1.5f, (float)(rand() % 101) / 100);
		v = v * velocity;
		//v *= scale;
		m_velData[3 * i] = v.x;
		m_velData[3 * i + 1] = v.y;
		m_velData[3 * i + 2] = v.z;

		m_timeData[i] = 0.0f;

		m_particles[i].Position = pos;
		m_particles[i].Velocity = v;
		m_particles[i].StartTime = m_timeData[i];
		m_particles[i].InitialVelocity = v;
		m_particles[i].InitialPosition = pos;
	}

	// CREATE BUFFER(S) AND BUFFER DATA HERE
	m_posBufferLocation = glGetAttribLocation(m_shader->GetShaderProgram(), "VertexPosition");
	m_timeBufferLocation = glGetAttribLocation(m_shader->GetShaderProgram(), "VertexStartTime");
	// buffer color as well? NO. Not per particle. Use uniform per particleEffect
	glGenBuffers(1, &m_posBuffer);
	// Allocate and buffer data
	glBindBuffer(GL_ARRAY_BUFFER, m_posBuffer);
	glBufferData(GL_ARRAY_BUFFER, m_nrParticles * 3 * sizeof(float), m_posData, GL_STATIC_DRAW);

	glGenBuffers(1, &m_timeBuffer);
	// Allocate and buffer data
	glBindBuffer(GL_ARRAY_BUFFER, m_timeBuffer);
	glBufferData(GL_ARRAY_BUFFER, m_nrParticles * sizeof(float), m_timeData, GL_STATIC_DRAW);
}


Explosion::~Explosion()
{
	//Delete eventuella buffers osv
}

void Explosion::Render(float _dt)
{
	glBlendFunc(GL_SRC_ALPHA, m_dstBlendFactor);

	m_elapsedTime += _dt;

	if (m_endPhase == 1)
		m_removeDelayTime += _dt;

	//DO THE UPDATE CALCULATIONS HERE
	for (int i = 0; i < m_nrParticles; i++)
	{
		if (m_elapsedTime >= m_particles[i].StartTime)
		{
			float age = m_elapsedTime - m_particles[i].StartTime;

			if (age >m_lifeTime && m_endPhase == 0)
			{
				// The particle is past its lifetime, recycle.
				m_particles[i].Position = m_particles[i].InitialPosition;
				m_particles[i].Velocity = m_particles[i].InitialVelocity;
				m_particles[i].StartTime = m_elapsedTime - (age - m_lifeTime);
			}
			else
			{
				// The particle is alive, update.
				m_particles[i].Position += m_particles[i].Velocity * _dt;
				float velMultiplier = std::max(1.0f - ((m_elapsedTime - m_particles[i].StartTime) / m_lifeTime), 0.85f);
				m_particles[i].Velocity *= (velMultiplier*velMultiplier);

				m_particles[i].Velocity += m_accel * _dt;
			}
			m_posData[3 * i + 0] = m_particles[i].Position.x;
			m_posData[3 * i + 1] = m_particles[i].Position.y;
			m_posData[3 * i + 2] = m_particles[i].Position.z;

			m_timeData[i] = m_particles[i].StartTime;
		}
	}

	// Set the uniforms: H and Time
	m_shader->SetUniVariable("Time", glfloat, &m_elapsedTime);
	m_shader->SetUniVariable("ParticleLifetime", glfloat, &m_lifeTime);
	m_shader->SetUniVariable("Size", glfloat, &m_spriteSize);
	//m_shader->SetUniVariable("EndPhase", glint, &m_endPhase);

	// ---Render---
	// Allocate and buffer data
	glBindBuffer(GL_ARRAY_BUFFER, m_posBuffer);
	glBufferData(GL_ARRAY_BUFFER, m_nrParticles * 3 * sizeof(float), m_posData, GL_STATIC_DRAW);
	glVertexAttribPointer(m_posBufferLocation, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(m_posBufferLocation);

	glBindBuffer(GL_ARRAY_BUFFER, m_timeBuffer);
	glBufferData(GL_ARRAY_BUFFER, m_nrParticles * sizeof(float), m_timeData, GL_STATIC_DRAW);
	glVertexAttribPointer(m_timeBufferLocation, 1, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(m_timeBufferLocation);

	glDrawArrays(GL_POINTS, 0, m_nrParticles);
}
