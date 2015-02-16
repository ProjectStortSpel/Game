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

void Model::Draw(mat4 viewMatrix, mat4 projectionMatrix)
{
	std::vector<mat4> MVPVector(instances.size());
	std::vector<mat3> normalMatVector(instances.size());
	std::vector<float> colors(instances.size() * 4);

	int nrOfInstances = 0;

	for (int j = 0; j < instances.size(); j++)
	{
		if (instances[j].active) // IS MODEL ACTIVE?
		{
			mat4 modelMatrix;
			if (instances[j].modelMatrix == NULL)
				modelMatrix = glm::translate(glm::vec3(1));
			else
				modelMatrix = *instances[j].modelMatrix;

			mat4 modelViewMatrix;
			modelViewMatrix = viewMatrix * modelMatrix;

			mat4 mvp = projectionMatrix * modelViewMatrix;
			MVPVector[nrOfInstances] = mvp;

			mat3 normalMatrix = glm::transpose(glm::inverse(mat3(modelViewMatrix)));
			normalMatVector[nrOfInstances] = normalMatrix;

			colors[4 * nrOfInstances + 0] = instances[j].color[0];
			colors[4 * nrOfInstances + 1] = instances[j].color[1];
			colors[4 * nrOfInstances + 2] = instances[j].color[2];
			colors[4 * nrOfInstances + 3] = 0.f;

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
	bufferPtr->drawInstanced(0, nrOfInstances, &MVPVector, &normalMatVector, &colors);
	glBindTexture(GL_TEXTURE_2D, 0);
}

bool Model::Compare(Model m)
{
	if (texID != m.texID) return false;
	if (bufferPtr != m.bufferPtr) return false;
	if (speID != m.speID) return false;
	if (norID != m.norID) return false;
	return true;
}