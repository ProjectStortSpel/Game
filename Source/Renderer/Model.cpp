#include "Model.h"
using namespace Renderer;
using namespace glm;

Model::Model(Buffer* buffer, GLuint tex, GLuint nor, GLuint spe, bool shadow)
{
	bufferPtr = buffer;
	texID = tex;
	norID = nor;
	speID = spe;
	castShadow = shadow;
}

Model::Model()
{
}

Model::~Model()
{
}

void Model::Draw(mat4 viewMatrix)
{
	std::vector<mat4> MVVector(instances.size());
	std::vector<mat3> normalMatVector(instances.size());
	float* colors = new float[instances.size() * 3];

	int nrOfInstances = 0;

	for (int j = 0; j < instances.size(); j++)
	{
		if (instances[j].active) // IS MODEL ACTIVE?
		{
			mat4 modelMatrix;
			if (instances[j].modelMatrix == NULL)
				modelMatrix = glm::mat4(1.0f);
			else
				modelMatrix = *instances[j].modelMatrix;

			mat4 modelViewMatrix = mat4();
			modelViewMatrix = viewMatrix * modelMatrix;

			MVVector[nrOfInstances] = modelViewMatrix;

			mat3 normalMatrix = glm::transpose(glm::inverse(mat3(modelViewMatrix)));
			normalMatVector[nrOfInstances] = normalMatrix;

			colors[j * 3 + 0] = instances[j].color[0];
			colors[j * 3 + 1] = instances[j].color[1];
			colors[j * 3 + 2] = instances[j].color[2];

			nrOfInstances++;
		}
	}

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texID);

	if (norID != 0) 
	{
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, norID);
	}
	if (speID != 0)
	{
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, speID);
	}
	//m_modelsDeferred[i].bufferPtr->draw();
	bufferPtr->drawInstanced(0, nrOfInstances, &MVVector, &normalMatVector, colors);
	glBindTexture(GL_TEXTURE_2D, 0);
	delete [] colors;
}

bool Model::Compare(Model m)
{
	if (texID != m.texID) return false;
	if (bufferPtr != m.bufferPtr) return false;
	if (speID != m.speID) return false;
	if (norID != m.norID) return false;
	return true;
}