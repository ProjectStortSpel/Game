#include "ParticleEffect.h"

using namespace Renderer;

ParticleEffect::ParticleEffect()
{
}

ParticleEffect::ParticleEffect(const vec3 _pos, int _nParticles, float _lifeTime, float _scale, float _spriteSize, GLuint _texHandle, vec3 _color, Shader *_shaderProg)
{
	m_pos = _pos;
	m_nrParticles = std::min(_nParticles, 20);
	m_scale = _scale;
	m_spriteSize = _spriteSize;
	m_lifeTime = _lifeTime;
	m_textureHandle = _texHandle;
	m_shader = _shaderProg;
	m_color = _color;
	m_endPhase = 0;

	m_elapsedTime = 0.0f;
	m_removeDelayTime = 0.0f;
	//--------------------------------------
}


ParticleEffect::~ParticleEffect()
{
	delete m_posData;
	delete m_velData;
	delete m_timeData;

	delete m_particles;

	//Delete eventuella buffers osv
	glDeleteBuffers(1, &m_posBuffer);
	glDeleteBuffers(1, &m_timeBuffer);
}

void ParticleEffect::EnterEndPhase()
{
	m_endPhase = 1;
}

bool ParticleEffect::ReadyToBeDeleted()
{
	if (m_removeDelayTime > m_lifeTime)
		return true;

	return false;
}