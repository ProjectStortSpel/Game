#ifndef PARTICLEEFFECT_H
#define PARTICLEEFFECT_H
#include "../stdafx.h"
#include "../Shader.h"
#include <time.h>

using glm::vec3;

namespace Renderer
{
	struct Particle{
		vec3 Position;
		vec3 Velocity;
		float StartTime;
		vec3 InitialVelocity;
		vec3 InitialPosition;
	};
	class DECLSPEC ParticleEffect
	{
	public:
		ParticleEffect();
		ParticleEffect(const vec3 _pos, const vec3 _vel, int _nParticles, float _lifeTime, vec3 _scale, float _spriteSize, GLuint _texHandle, vec3 _color, Shader *_shaderProg);
		~ParticleEffect();

		GLuint GetTexHandle(){ return m_textureHandle; }

		virtual void Render(float _deltaTimeMillis) = 0;
		vec3 GetWorldPos(){ return m_pos; }
		vec3 GetAccel(){ return m_accel; }
		void SetAccel(vec3 _acc){ m_accel = _acc; }
		vec3* GetColor(){ return &m_color; }
		Shader* GetShaderPtr(){ return m_shader; }
		void EnterEndPhase();
		bool ReadyToBeDeleted();

	protected:
		Shader *m_shader;

		int m_nrParticles;
		vec3 m_pos, m_vel, m_accel, m_color;
		float m_spriteSize, m_lifeTime;
		float m_elapsedTime, m_removeDelayTime;
		vec3 m_scale;
		int m_endPhase;
		GLenum m_dstBlendFactor;

		GLuint m_textureHandle;

		GLfloat *m_posData, *m_velData, *m_timeData;

		struct Particle *m_particles;

		GLuint m_posBuffer, m_timeBuffer;
		GLuint m_posBufferLocation, m_timeBufferLocation;
	};
}
#endif
