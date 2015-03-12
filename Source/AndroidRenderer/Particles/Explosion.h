#ifndef EXPLOSION_H
#define EXPLOSION_H
#include "ParticleEffect.h"

using glm::vec3;

namespace Renderer
{
	class DECLSPEC Explosion : public ParticleEffect
	{
	public:
		Explosion();
		Explosion(const vec3 _pos, const vec3 _vel, int _nParticles, float _lifeTime, vec3 _scale, float _spriteSize, GLuint _texHandle, vec3 _color, Shader *_shaderProg);
		~Explosion();

		void Render(float _deltaTimeMillis);
		
	private:
		//variables
	};
}
#endif
