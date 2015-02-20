#ifndef AMODEL_H
#define AMODEL_H
#include "stdafx.h"
#include "Buffer.h"

namespace Renderer
{
	struct Joint
	{
		float x0, y0, z0, w0;
		float x1, y1, z1, w1;
		float x2, y2, z2, w2;
		float x3, y3, z3, parent;
		Joint(	float _x0, float _y0, float _z0, float _w0,
					float _x1, float _y1, float _z1, float _w1,
					float _x2, float _y2, float _z2, float _w2,
					float _x3, float _y3, float _z3, float _parent)
		{
			x0 = _x0;
			y0 = _y0;
			z0 = _z0;
			w0 = _w0;
			x1 = _x1;
			y1 = _y1;
			z1 = _z1;
			w1 = _w1;
			x2 = _x2;
			y2 = _y2;
			z2 = _z2;
			w2 = _w2;
			x3 = _x3;
			y3 = _y3;
			z3 = _z3;
			parent = _parent;
		}
	};

	class DECLSPEC AModel
	{
	public:
		AModel(int _id, bool _active, glm::mat4* _model, float* _color, Buffer* buffer, GLuint tex, GLuint nor, GLuint spe);
		AModel();
		~AModel();

		int id;
		bool active;
		glm::mat4* modelMatrix;
		float* color;
		Buffer* bufferPtr;
		GLuint texID;
		GLuint norID;
		GLuint speID;
		GLuint jointBuffer;
		std::vector<Joint> joints;
	};
}

#endif 

