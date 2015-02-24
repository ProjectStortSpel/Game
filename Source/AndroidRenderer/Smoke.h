#ifndef SMOKE_H
#define SMOKE_H
#include "ParticleEffect.h"

using glm::vec3;

namespace Renderer
{
	class DECLSPEC Smoke : public ParticleEffect
	{
	public:
		Smoke();
		Smoke(const vec3 _pos, int _nParticles, float _lifeTime, float _scale, float _spriteSize, GLuint _texHandle, vec3 _color, Shader *_shaderProg);
		~Smoke();

		void Render(float _deltaTimeMillis);
		
	private:
		//variables
	};
}
#endif
