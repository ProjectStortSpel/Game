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
	std::string jonts;
	std::vector<std::string> anim;
	ObjectData(std::string dir)
	{
		animated = false;
		mesh = text = norm = spec = jonts = dir;
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
		objectdata.jonts.append(temp);

		while (getline(fileIn, temp))
		{ 
			std::string animation;
			animation = fileDir;
			animation.append(temp);
			objectdata.anim.push_back(animation);
		}

		return objectdata;
	}
};

#endif