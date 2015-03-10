#ifndef WATERFALL_H
#define WATERFALL_H
#include "ParticleEffect.h"

using glm::vec3;

namespace Renderer
{
	class DECLSPEC Waterfall : public ParticleEffect
	{
	public:
		Waterfall();
		Waterfall(const vec3 _pos, const vec3 _vel, int _nParticles, float _lifeTime, vec3 _scale, float _spriteSize, GLuint _texHandle, vec3 _color, Shader *_shaderProg);
		~Waterfall();

		void Render(float _deltaTimeMillis);
		
	private:
		//variables
	};
}
#endif
