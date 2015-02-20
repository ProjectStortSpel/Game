#ifndef PARTICLESYSTEM_H
#define PARTICLESYSTEM_H
#include "stdafx.h"
#include "Shader.h"
#include <time.h>

using glm::vec3;

namespace Renderer
{
	class ParticleSystem
	{
	public:
		ParticleSystem(std::string _type, const vec3 _pos, int _nParticles, float _lifeTime, float _scale, float _spriteSize, GLuint _texHandle, vec3 _color, Shader *_shaderProg);
		~ParticleSystem();

		GLuint GetTexHandle(){ return m_textureHandle; }

		bool InitParticleSystem(const vec3 _pos, int _nParticles, char* _texPath);
		void Render(float _deltaTimeMillis);
		vec3 GetWorldPos(){ return m_pos; }
		vec3 GetAccel(){ return m_accel; }
		void SetAccel(vec3 _acc){ m_accel = _acc; }
		vec3* GetColor(){ return &m_color; }
		Shader* GetShaderPtr(){ return m_shader; }
		void EnterEndPhase();
		bool ReadyToBeDeleted();

	private:
		Shader *m_shader;

		GLuint m_particleArray[2];
		GLuint m_feedback[2]; // Transform feedback objects
		GLuint m_posBuf[2]; // Position buffers (A and B)
		GLuint m_velBuf[2]; // Velocity buffers (A and B)
		GLuint m_startTime[2]; // Start time buffers (A and B)
		GLuint m_initVelBuf;
		GLuint m_initPosBuf;

		GLuint subRoutineUpdate, subRoutineRender;

		GLuint m_textureHandle;

		GLuint vboHandles[8];

		int m_nrParticles;
		int m_drawBuf;
		vec3 m_pos, m_accel, m_color;
		float m_spriteSize, m_lifeTime;
		float m_elapsedTime, m_removeDelayTime;
		float m_scale;
		int m_endPhase;
		GLenum m_dstBlendFactor;

		void CreateFire();
		void CreateSmoke();
	};
}
#endif