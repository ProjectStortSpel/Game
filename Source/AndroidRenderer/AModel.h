#ifndef AMODEL_H
#define AMODEL_H
#include "stdafx.h"
#include "Buffer.h"
#include "Shader.h"

namespace Renderer
{
	struct Joint
	{
		float parent;
		Joint(float _parent)
		{
			parent = _parent;
		}
	};

	struct KeyFrame
	{
		int frame;
		glm::mat4 mat;
		KeyFrame(int _frame, glm::mat4 _mat)
		{
			frame = _frame;
			mat = _mat;
		}
	};

	struct JointAnim
	{
		int jointId;
		int* lastFrame;
		std::vector<KeyFrame> keyFrames;
		std::vector<glm::mat4> frames;
		JointAnim(int _jointId, int* _lastFrame)
		{
			jointId = _jointId;
			lastFrame = _lastFrame;
		}
		glm::mat4 getFrame(int frame)
		{
			if (keyFrames.size() == 1)
			{
				return keyFrames[0].mat;
			}
			else if (keyFrames.size() > 1)
			{
				for (int i = 0; i < keyFrames.size(); i++)
				{
					if (frame <= keyFrames[i].frame)
					{
						int j = (i - 1 + keyFrames.size()) % keyFrames.size();
						int nextKeyFrame = keyFrames[i].frame;
						int prevKeyFrame = keyFrames[j].frame;
						if (nextKeyFrame < prevKeyFrame) nextKeyFrame += *lastFrame;
						float span = nextKeyFrame - prevKeyFrame;
						int interframe = frame - prevKeyFrame;

						float proc = interframe / span;

						glm::mat4 mat = keyFrames[i].mat * proc;
						mat += keyFrames[j].mat * (1.f - proc);
						
						return mat;
					}
				}
			}
			return glm::mat4(1);
		}
	};

	struct Animation
	{
		std::string name;
		int maxFrame;
		std::vector<JointAnim> joints;
		Animation(std::string _name)
		{
			name = _name;
			maxFrame = 0;
		}
	};

	class DECLSPEC AModel
	{
	public:
		std::string name;

		AModel(int _id, bool _active, glm::mat4* _model, float* _color, Buffer* buffer, GLuint tex, GLuint nor, GLuint spe);
		AModel();
		~AModel();

		void Draw(glm::mat4 viewMatrix, glm::mat4 projectionMatrix, Shader* shaderptr);
		void DrawGeometry(glm::mat4 viewMatrix, glm::mat4 projectionMatrix, Shader* shaderptr);
		void DrawForwardGeometry(glm::mat4 viewMatrix, glm::mat4 projectionMatrix, Shader* shaderptr);
		void Update(float _dt);

		bool PreCalculateAnimations();

		bool SetAnimation(int _animId);
		void SetFrameTime(float _animId);

		void AddKeyFrame(std::string _animname, int _frame, int _joint, glm::mat4 _mat);

		int id;
		bool active;
		glm::mat4* modelMatrix;
		float* color;
		Buffer* bufferPtr;
		GLuint texID;
		GLuint norID;
		GLuint speID;
		//GLuint jointBuffer;
		GLuint* animBuffer;
		std::vector<glm::mat4> extra;
		std::vector<glm::mat4> joints;
		std::vector<glm::mat4> anim;
		std::vector<Joint> animation;
		std::vector<Animation> animations;
		int animId;

		int currentframe;
		float clock;
		float frameTime;
		int framesPerTick;
	private:
		Animation* GetAnimationPointer(std::string _animname);
		JointAnim* GetJointAnimPointer(int _jointId, Animation* _animptr);
	};
}

#endif 

