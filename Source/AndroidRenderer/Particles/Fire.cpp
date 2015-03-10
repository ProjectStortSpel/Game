#include "Fire.h"

using namespace Renderer;

Fire::Fire()
{
}

Fire::Fire(const vec3 _pos, const vec3 _vel, int _nParticles, float _lifeTime, vec3 _scale, float _spriteSize, GLuint _texHandle, vec3 _color, Shader *_shaderProg) : ParticleEffect(_pos, _vel, _nParticles, _lifeTime, _scale, _spriteSize, _texHandle, _color, _shaderProg)
{
	//----Init starts here------------------
	//..-----FIRE-----..
	m_dstBlendFactor = GL_CONSTANT_COLOR;

	m_accel = vec3(0.0);
	vec3 v(0.0f);
	float velocity, theta, phi;
	float mtime = 0.0f, rate = (m_lifeTime / (float)m_nrParticles);//0.00075f;

	m_posData = new GLfloat[m_nrParticles * 3];
	m_velData = new GLfloat[m_nrParticles * 3];
	m_timeData = new GLfloat[m_nrParticles];

	m_particles = new Particle[m_nrParticles];

	//SDL_Log("m_nrParticles: %d", m_nrParticles);
	srand(time(NULL));
	for (GLuint i = 0; i < m_nrParticles; i++) {
		vec3 pos;
		pos = vec3(((float)(rand() % 9) - 4), 0.0f, ((float)(rand() % 9 - 4)));
		pos = glm::normalize(pos) * ((float)(rand() % 9 - 4)) * 0.5f;
		pos.x *= m_scale.x;
		pos.y *= m_scale.y;
		pos.z *= m_scale.z;

		m_posData[3 * i] = pos.x; //((float)(rand() % 9) - 4) * scale;
		m_posData[3 * i + 1] = pos.y; // 0.0;
		m_posData[3 * i + 2] = pos.z; // ((float)(rand() % 9 - 4)) * scale;

		// Pick the direction of the velocity
		theta = glm::mix(0.0f, (float)M_PI / 6.0f, (float)(rand() % 101) / 100);
		phi = glm::mix(0.0f, (float)(2 * M_PI), (float)(rand() % 101) / 100);

		v.x = sinf(theta) * cosf(phi) * 0.1;
		if ((m_posData[3 * i] < 0 && v.x < 0) || (m_posData[3 * i] > 0 && v.x > 0))
			v.x *= -1;
		v.y = cosf(theta) * 0.2;
		v.z = sinf(theta) * sinf(phi) * 0.1;
		if ((m_posData[3 * i + 2] < 0 && v.z < 0) || (m_posData[3 * i + 2] > 0 && v.z > 0))
			v.z *= -1;

		// Scale to set the magnitude of the velocity (speed)
		velocity = glm::mix(1.25f, 1.5f, (float)(rand() % 101) / 100) * 1.2;
		v = v * velocity + m_vel;
		m_velData[3 * i] = v.x;
		m_velData[3 * i + 1] = v.y;
		m_velData[3 * i + 2] = v.z;

		m_timeData[i] = mtime;
		mtime += rate;
		
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


Fire::~Fire()
{
	//Delete eventuella buffers osv
}

void Fire::Render(float _dt)
{
	glBlendColor(0.93, 0.93, 0.93, 1.0);
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
