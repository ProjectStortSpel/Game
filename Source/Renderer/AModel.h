#ifndef AMODEL_H
#define AMODEL_H
#include "stdafx.h"
#include "Buffer.h"
#include "Shader.h"

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
				float _x3, float _y3, float _z3, float _parent	)
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
		}
	};

	class DECLSPEC AModel
	{
	public:
		AModel(int _id, bool _active, glm::mat4* _model, float* _color, Buffer* buffer, GLuint tex, GLuint nor, GLuint spe);
		AModel();
		~AModel();

		void Draw(glm::mat4 viewMatrix, glm::mat4 projectionMatrix, Shader* shaderptr);
		void Update(float _dt);

		bool SetAnimation(int _animId);

		void AddKeyFrame(std::string _animname, int _frame, int _joint, glm::mat4 _mat);

		int id;
		bool active;
		glm::mat4* modelMatrix;
		float* color;
		Buffer* bufferPtr;
		GLuint texID;
		GLuint norID;
		GLuint speID;
		GLuint jointBuffer;
		GLuint animBuffer;
		std::vector<Joint> joints;
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

