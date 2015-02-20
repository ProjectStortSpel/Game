#include "AModel.h"
using namespace Renderer;
using namespace glm;

AModel::AModel(int _id, bool _active, mat4* _model, float* _color, Buffer* buffer, GLuint tex, GLuint nor, GLuint spe)
{
	id = _id;
	active = _active;
	modelMatrix = _model;
	color = _color;
	bufferPtr = buffer;
	texID = tex;
	norID = nor;
	speID = spe;
}

AModel::AModel()
{
}

AModel::~AModel()
{
	glDeleteBuffers(1, &jointBuffer);
}