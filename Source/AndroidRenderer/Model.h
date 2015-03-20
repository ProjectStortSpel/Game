#ifndef MODEL_H
#define MODEL_H
#include "stdafx.h"
#include "Buffer.h"
#include "Shader.h"

namespace Renderer
{
	class DECLSPEC Model
	{
	public:
		std::string name;

		bool operator== (const Model &m) { return Compare(m); }
		bool operator!= (const Model &m) { return !Compare(m); }

		Model(Buffer* buffer, GLuint tex, GLuint nor, GLuint spe, int id, bool active, glm::mat4* model, float* col, bool shadow);
		Model();
		~Model();

		void Draw(glm::mat4 viewMatrix, Shader* shader);

		Buffer* bufferPtr;
		GLuint texID;
		GLuint norID;
		GLuint speID;
		
		int id;
		bool active;
		glm::mat4* modelMatrix;
		float* color;
		bool castShadow;

	private:
		bool Compare(Model m);
	};
}

#endif 

