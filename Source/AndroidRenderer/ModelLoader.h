#ifndef MODELLOADER_H
#define MODELLOADER_H
#include "stdafx.h"

struct ObjectData
{
	std::string mesh;
	std::string text;
	std::string norm;
	std::string spec;
	ObjectData(std::string dir)
	{
		mesh = text = norm = spec = dir;
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

		// Open file
		SDL_RWops* fileIn = SDL_RWFromFile(fileDir.c_str(), "r");
		if (fileIn == NULL)
		{
			SDL_Log("File %s not found", fileDir.c_str());
			return objectdata;
		}
		// Get file length
		Sint64 length = SDL_RWseek(fileIn, 0, RW_SEEK_END);
		if (length <= 0)
		{
			SDL_Log("Length of file %s lower than or equal to zero", fileDir.c_str());
			return objectdata;
		}
		SDL_RWseek(fileIn, 0, RW_SEEK_SET);
		// Read data
		char* data = new char[length + 1];
		SDL_RWread(fileIn, data, length, 1);
		data[length] = '\0';
		std::string dataString = std::string(data);
		delete data;
		// Close file
		SDL_RWclose(fileIn);

		//std::ifstream fileIn(fileDir);

		size_t prevFileIndex = 0, nextFileIndex = std::string::npos;
		std::string temp;
		nextFileIndex = dataString.find('\n', prevFileIndex);
		temp = dataString.substr(prevFileIndex, nextFileIndex - prevFileIndex);
		temp.erase(std::remove(temp.begin(), temp.end(), 13), temp.end());
		prevFileIndex = nextFileIndex + 1;
		objectdata.mesh.append(temp);
		nextFileIndex = dataString.find('\n', prevFileIndex);
		temp = dataString.substr(prevFileIndex, nextFileIndex - prevFileIndex);
		temp.erase(std::remove(temp.begin(), temp.end(), 13), temp.end());
		prevFileIndex = nextFileIndex + 1;
		objectdata.text.append(temp);
		nextFileIndex = dataString.find('\n', prevFileIndex);
		temp = dataString.substr(prevFileIndex, nextFileIndex - prevFileIndex);
		temp.erase(std::remove(temp.begin(), temp.end(), 13), temp.end());
		prevFileIndex = nextFileIndex + 1;
		objectdata.norm.append(temp);
		nextFileIndex = dataString.find('\n', prevFileIndex);
		temp = dataString.substr(prevFileIndex, nextFileIndex - prevFileIndex);
		temp.erase(std::remove(temp.begin(), temp.end(), 13), temp.end());
		prevFileIndex = nextFileIndex + 1;
		objectdata.spec.append(temp);

		return objectdata;
	}
};

#endif
