#include "Model.h"
using namespace Renderer;
using namespace glm;

Model::Model(Buffer* buffer, GLuint tex, GLuint nor, GLuint spe, int id, bool active, mat4* model, float* col, bool shadow)
{
	bufferPtr = buffer;
	texID = tex;
	norID = nor;
	speID = spe;
	
	this->id = id;
	this->active = active;
	this->modelMatrix = model;
	this->color = col;
	this->castShadow = shadow;
}

Model::Model()
{
}

Model::~Model()
{
}

void Model::Draw(mat4 viewMatrix, Shader* shader)
{
	if (this->active) // IS MODEL ACTIVE?
	{
		mat4 modelMatrix;
		if (this->modelMatrix == NULL)
			modelMatrix = glm::translate(glm::vec3(1));
		else
			modelMatrix = *this->modelMatrix;

		mat4 modelViewMatrix = viewMatrix * modelMatrix;
		shader->SetUniVariable("ModelViewMatrix", mat4x4, &modelViewMatrix);

		mat3 normalMatrix = glm::transpose(glm::inverse(mat3(modelViewMatrix)));
		shader->SetUniVariable("NormalMatrix", mat3x3, &normalMatrix);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, this->texID);

		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, this->norID);

		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, this->speID);

		shader->SetUniVariable("BlendColor", vector3, this->color);

		this->bufferPtr->draw(shader->GetShaderProgram());
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

bool Model::Compare(Model m)
{
	if (texID != m.texID) return false;
	if (bufferPtr != m.bufferPtr) return false;
	if (speID != m.speID) return false;
	if (norID != m.norID) return false;
	return true;
}