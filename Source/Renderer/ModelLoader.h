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
	float x0, y0, z0, w0;
	float x1, y1, z1, w1;
	float x2, y2, z2, w2;
	float x3, y3, z3, parent;
	JointData(	float _x0, float _y0, float _z0, float _w0, 
				float _x1, float _y1, float _z1, float _w1, 
				float _x2, float _y2, float _z2, float _w2, 
				float _x3, float _y3, float _z3, float _parent)
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
			animation = fileDir;
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
		std::vector<JointData> jointlist;

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

				jointlist.push_back(
					JointData(	xx, xy, xz, xw,
								yx, yy, yz, yw,
								zx, zy, zz, zw,
								wx, wy, wz, parent
								)
					);
			}
		}
		return jointlist;
	}
};

#endif