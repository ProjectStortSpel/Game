#ifndef MODEL_H
#define MODEL_H
#include "stdafx.h"
#include "Buffer.h"

namespace Renderer
{
	struct Instance
	{
		int id;
		bool active;
		bool viewspace;
		glm::mat4* modelMatrix;
		float* color;

		Instance(){}
		Instance(int _id, bool _active, glm::mat4* _model, float* _color)
		{
			id = _id;
			active = _active;
			modelMatrix = _model;
			color = _color;
		}
	};

	class DECLSPEC Model
	{
	public:
		std::string name;

		bool operator== (const Model &m) { return Compare(m); }
		bool operator!= (const Model &m) { return !Compare(m); }

		Model(Buffer* buffer, GLuint tex, GLuint nor, GLuint spe);
		Model();
		~Model();

		void Draw(glm::mat4 viewMatrix, glm::mat4 projectionMatrix);

		Buffer* bufferPtr;
		GLuint texID;
		GLuint norID;
		GLuint speID;

		std::vector<Instance> instances;

	private:
		bool Compare(Model m);
	};
}

#endif 

