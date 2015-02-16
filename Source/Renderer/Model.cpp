#include "Model.h"
using namespace Renderer;
using namespace glm;

Model::Model(Buffer* buffer, GLuint tex, GLuint nor, GLuint spe)
{
	bufferPtr = buffer;
	texID = tex;
	norID = nor;
	speID = spe;
}

Model::Model()
{
}

Model::~Model()
{
}

bool Model::Compare(Model m)
{
	if (texID != m.texID) return false;
	if (bufferPtr != m.bufferPtr) return false;
	if (speID != m.speID) return false;
	if (norID != m.norID) return false;
	return true;
}