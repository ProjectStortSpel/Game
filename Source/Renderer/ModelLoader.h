#ifndef MODELLOADER_H
#define MODELLOADER_H
#include "stdafx.h"

struct ObjectData
{
	bool animated;
	std::string mesh;
	std::string text;
	std::string norm;
	std::string spec;
	std::string joints;
	std::vector<std::string> anim;
	ObjectData(std::string dir)
	{
		animated = false;
		mesh = text = norm = spec = joints = dir;
	}
};

struct JointData
{
	glm::mat4 mat;
	float parent;
	JointData(glm::mat4 _mat, float _parent)
	{
		mat = _mat;
		parent = _parent;
	}
};

struct AnimData
{
	int frame;
	int joint;
	glm::mat4 mat;
	AnimData(int _frame, int _joint, glm::mat4 _mat)
	{
		frame = _frame;
		joint = _joint;
		mat = _mat;
	}
};

struct Vertex
{
	glm::vec3 po;
	glm::vec3 no;
	glm::vec3 ta;
	glm::vec3 bi;
	glm::vec2 uv;
	Vertex(glm::vec3 _p, glm::vec3 _n, glm::vec3 _t, glm::vec3 _b, glm::vec2 _u)
	{
		po = _p;
		no = _n;
		ta = _t;
		bi = _b;
		uv = _u;
	}
};

class ModelLoader{
public:
	static std::vector<Vertex> importMesh(std::string fileDir)
	{
		std::vector<Vertex> vertexlist;

		std::ifstream fileIn(fileDir.c_str());

		if (fileIn)
		{
			while (fileIn)
			{
				float px, py, pz;
				float nx, ny, nz;
				float tx, ty, tz;
				float bx, by, bz;
				float u, v;

				fileIn >> px >> py >> pz;
				fileIn >> nx >> ny >> nz;
				fileIn >> tx >> ty >> tz;
				fileIn >> bx >> by >> bz;
				fileIn >> u >> v;

				vertexlist.push_back(Vertex(
					glm::vec3(px, py, pz), 
					glm::vec3(nx, ny, nz),
					glm::vec3(tx, ty, tz),
					glm::vec3(bx, by, bz),
					glm::vec2(u, v)
				));
			}
		}

		return vertexlist;
	}

	static ObjectData importObject(std::string dir, std::string file)
	{
		ObjectData objectdata(dir);

		std::string fileDir = dir;
		fileDir.append(file);

		std::ifstream fileIn(fileDir);

		std::string temp;
		getline(fileIn, temp);
		objectdata.mesh.append(temp);
		getline(fileIn, temp);
		objectdata.text.append(temp);
		getline(fileIn, temp);
		objectdata.norm.append(temp);
		getline(fileIn, temp);
		objectdata.spec.append(temp);

		getline(fileIn, temp);
		objectdata.joints.append(temp);

		while (getline(fileIn, temp))
		{ 
			std::string animation;
			animation = dir;
			animation.append(temp);
			objectdata.anim.push_back(animation);
		}
		int isamesh = objectdata.mesh.find(".amesh");
		if (isamesh > 0 && objectdata.joints != fileDir)
		{
			objectdata.animated = true;
		}
			

		return objectdata;
	}


	static std::vector<JointData> importJoints(std::string fileDir)
	{
		std::vector<JointData> jointlistTemp;

		std::ifstream fileIn(fileDir.c_str());

		if (fileIn)
		{
			while (fileIn)
			{
				int parent;
				float xx, xy, xz, xw;
				float yx, yy, yz, yw;
				float zx, zy, zz, zw;
				float wx, wy, wz, ww;

				fileIn >> parent;
				fileIn >> xx >> xy >> xz >> xw;
				fileIn >> yx >> yy >> yz >> yw;
				fileIn >> zx >> zy >> zz >> zw;
				fileIn >> wx >> wy >> wz >> ww;

				glm::mat4 matrix = glm::mat4(xx, xy, xz, xw,
					yx, yy, yz, yw,
					zx, zy, zz, zw,
					wx, wy, wz, ww);

				jointlistTemp.push_back(JointData(matrix, parent));
			}
		}

		for (int i = 0; i < jointlistTemp.size(); i++)
		{
			if (jointlistTemp[i].parent >= 0)
			{
				jointlistTemp[i].mat = jointlistTemp[int(jointlistTemp[i].parent)].mat * jointlistTemp[i].mat;
			}
		}

		for (int i = 0; i < jointlistTemp.size(); i++)
		{
			jointlistTemp[i].mat = glm::inverse(jointlistTemp[i].mat);
		}

		return jointlistTemp;
	}

	static std::vector<AnimData> importAnimation(std::string fileDir)
	{
		std::vector<AnimData> jointlistTemp;
	
		std::ifstream fileIn(fileDir.c_str());
	
		if (fileIn)
		{
			while (fileIn)
			{
				int joint;
				int frame;
				float xx, xy, xz, xw;
				float yx, yy, yz, yw;
				float zx, zy, zz, zw;
				float wx, wy, wz, ww;
	
				fileIn >> joint;
				fileIn >> frame;
				fileIn >> xx >> xy >> xz >> xw;
				fileIn >> yx >> yy >> yz >> yw;
				fileIn >> zx >> zy >> zz >> zw;
				fileIn >> wx >> wy >> wz >> ww;
	
				glm::mat4 matrix = glm::mat4(xx, xy, xz, xw,
					yx, yy, yz, yw,
					zx, zy, zz, zw,
					wx, wy, wz, ww);

				jointlistTemp.push_back(AnimData(frame, joint, matrix));
			}
		}

		return jointlistTemp;
	}
};

#endif