#ifndef FIRE_H
#define FIRE_H
#include "ParticleEffect.h"

using glm::vec3;

namespace Renderer
{
	class DECLSPEC Fire : public ParticleEffect
	{
	public:
		Fire();
		Fire(const vec3 _pos, int _nParticles, float _lifeTime, float _scale, float _spriteSize, GLuint _texHandle, vec3 _color, Shader *_shaderProg);
		~Fire();

		void Render(float _deltaTimeMillis);

	private:
		//variables
	};
}
#endif
