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
	currentframe = 1;
	clock = 0;
	frameTime = 0.01f;
	framesPerTick = 4;
	animId = 0;
}

AModel::AModel()
{
}

AModel::~AModel()
{
	glDeleteBuffers(1, &jointBuffer);
}

void AModel::Draw(mat4 viewMatrix, mat4 projectionMatrix, Shader* shaderptr)
{
	if (active) // IS MODEL ACTIVE?
	{
		mat4 M;
		if (modelMatrix == NULL)
			M = glm::translate(glm::vec3(1));
		else
			M = *modelMatrix;

		mat4 vp = projectionMatrix * viewMatrix;
		mat4 modelViewMatrix = M;
		mat3 normalMatrix = glm::transpose(glm::inverse(mat3(modelViewMatrix)));

		shaderptr->SetUniVariable("BlendColor", vector3, color);
		shaderptr->SetUniVariable("M", mat4x4, &modelViewMatrix);
		shaderptr->SetUniVariable("VP", mat4x4, &vp);
		shaderptr->SetUniVariable("NormalMatrix", mat3x3, &normalMatrix);




		float *joint_data = new float[joints.size() * 16];

		for (int j = 0; j < joints.size(); j++)
		{
			memcpy(&joint_data[16 * j], &joints[j], 16 * sizeof(float));
		}

		int joint_data_size = 16 * joints.size() * sizeof(float);

		glBindBufferRange(GL_SHADER_STORAGE_BUFFER, 1, jointBuffer, 0, joint_data_size);
		glBufferData(GL_SHADER_STORAGE_BUFFER, joint_data_size, joint_data, GL_STATIC_DRAW);

		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, jointBuffer);

		delete [] joint_data;




		float *anim_data = new float[animation.size() * 16];

		for (int j = 0; j < animation.size(); j++)
		{
			memcpy(&anim_data[16 * j], &animation[j], 16 * sizeof(float));
		}

		int anim_data_size = 16 * animation.size() * sizeof(float);

		glBindBufferRange(GL_SHADER_STORAGE_BUFFER, 2, animBuffer, 0, anim_data_size);
		glBufferData(GL_SHADER_STORAGE_BUFFER, anim_data_size, anim_data, GL_STATIC_DRAW);

		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, animBuffer);

		delete [] anim_data;



		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texID);

		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, norID);

		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, speID);

		bufferPtr->draw();

		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

void AModel::Update(float _dt)
{
	clock += _dt;
	if (clock > frameTime * framesPerTick)
	{
		clock -= frameTime * framesPerTick;
		currentframe += framesPerTick;
		currentframe = currentframe % animations[animId].maxFrame;

		Animation* animptr = &animations[animId];
		for (int i = 0; i < animptr->joints.size(); i++)
		{
			int index = animation.size() - animptr->joints[i].jointId - 1;
			glm::mat4 mat = animptr->joints[i].getFrame(currentframe + 1);
			animation[index] = Joint(
				mat[0][0], mat[0][1], mat[0][2], mat[0][3],
				mat[1][0], mat[1][1], mat[1][2], mat[1][3],
				mat[2][0], mat[2][1], mat[2][2], mat[2][3],
				mat[3][0], mat[3][1], mat[3][2], animation[index].parent
				);
		}
	}
}

bool AModel::SetAnimation(int _animId)
{
	if (_animId >= 0 && _animId < animations.size())
	{
		currentframe = 1;
		animId = _animId;
		return true;
	}
	return false;
}

void AModel::AddKeyFrame(std::string _animname, int _frame, int _joint, glm::mat4 _mat)
{
	Animation* animptr = GetAnimationPointer(_animname);
	JointAnim* jointptr = GetJointAnimPointer(_joint, animptr);

	if (_frame > animptr->maxFrame)
		animptr->maxFrame = _frame;

	jointptr->keyFrames.push_back(KeyFrame(_frame, _mat));
}

Animation* AModel::GetAnimationPointer(std::string _animname)
{
	for (int i = 0; i < animations.size(); i++)
	{
		if (animations[i].name == _animname)
		{
			return &animations[i];
		}
	}
	
	// if not found
	animations.push_back(Animation(_animname));
	return &animations[animations.size()-1];
}

JointAnim* AModel::GetJointAnimPointer(int _jointId, Animation* _animptr)
{
	for (int i = 0; i < _animptr->joints.size(); i++)
	{
		if (_animptr->joints[i].jointId == _jointId)
		{
			return &_animptr->joints[i];
		}
	}

	// if not found
	_animptr->joints.push_back(JointAnim(_jointId, &_animptr->maxFrame));
	return &_animptr->joints[_animptr->joints.size() - 1];
}