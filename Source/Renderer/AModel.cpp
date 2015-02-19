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
}

AModel::AModel()
{
}

AModel::~AModel()
{
	glDeleteBuffers(1, &jointBuffer);
}

void AModel::Update(float _dt)
{
	clock += _dt;
	if (clock > 0.04f)
	{
		clock -= 0.04f;
		currentframe++;
		currentframe = currentframe % animations[0].maxFrame;
	}

	Animation* animptr = &animations[0];
	
	for (int i = 0; i < animptr->joints.size(); i++)
	{
		for (int j = 0; j < animptr->joints[i].keyFrames.size(); j++)
		{
			if ((currentframe+1) == animptr->joints[i].keyFrames[j].frame)
			{
				glm::mat4 *mat = &animptr->joints[i].keyFrames[j].mat;
				int index = animation.size() - animptr->joints[i].jointId - 1;
				animation[index] = Joint(
					(*mat)[0][0], (*mat)[0][1], (*mat)[0][2], (*mat)[0][3],
					(*mat)[1][0], (*mat)[1][1], (*mat)[1][2], (*mat)[1][3],
					(*mat)[2][0], (*mat)[2][1], (*mat)[2][2], (*mat)[2][3],
					(*mat)[3][0], (*mat)[3][1], (*mat)[3][2], animation[index].parent
					);
			}
		}
	}
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
	_animptr->joints.push_back(JointAnim(_jointId));
	return &_animptr->joints[_animptr->joints.size() - 1];
}