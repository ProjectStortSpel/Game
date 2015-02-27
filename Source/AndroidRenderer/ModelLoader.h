#ifndef MODELLOADER_H
#define MODELLOADER_H
#include "stdafx.h"

struct ObjectData
{
	std::string mesh;
	std::string text;
	std::string norm;
	std::string spec;
	ObjectData()
	{
		mesh = text = norm = spec = "";
	}
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

	static bool GetFilePath(std::vector<std::string>& dirs, std::string& file, std::string* out)
	{
		for (int i = 0; i < dirs.size(); ++i)
		{
			std::string fileDir = dirs[i];
			fileDir.append(file);

			SDL_RWops* fileIn = SDL_RWFromFile(fileDir.c_str(), "r");
			if (fileIn != NULL)
			{
				Sint64 length = SDL_RWseek(fileIn, 0, RW_SEEK_END);
				SDL_RWclose(fileIn);
				if (length == 0)
				{
					*out = "";
					SDL_Log("Length of file %s lower than or equal to zero", fileDir.c_str());
					return false;
				}
				*out = fileDir;
				return true;
			}
		}
		*out = "";
		SDL_Log("File %s not found", file.c_str());
		return false;
	}

	static ObjectData importObject(std::vector<std::string> _dirs, std::string file)
	{
		ObjectData objectdata;

		std::string filePath;


		if (GetFilePath(_dirs, file, &filePath))
		{
			SDL_RWops* fileIn = SDL_RWFromFile(filePath.c_str(), "r");
			Sint64 length = SDL_RWseek(fileIn, 0, RW_SEEK_END);
			SDL_RWseek(fileIn, 0, RW_SEEK_SET);
			// Read data
			char* data = new char[length];
			SDL_RWread(fileIn, data, length, 1);
			std::string dataString = std::string(data, length);
			delete[] data;
			// Close file
			SDL_RWclose(fileIn);


			size_t prevFileIndex = 0, nextFileIndex = std::string::npos;
			std::string temp;


			nextFileIndex = dataString.find('\n', prevFileIndex);
			temp = dataString.substr(prevFileIndex, nextFileIndex - prevFileIndex);
			temp.erase(std::remove(temp.begin(), temp.end(), 13), temp.end());
			prevFileIndex = nextFileIndex + 1;
			if (GetFilePath(_dirs, temp, &filePath))
			{
				objectdata.mesh = filePath;
			}

			nextFileIndex = dataString.find('\n', prevFileIndex);
			temp = dataString.substr(prevFileIndex, nextFileIndex - prevFileIndex);
			temp.erase(std::remove(temp.begin(), temp.end(), 13), temp.end());
			prevFileIndex = nextFileIndex + 1;
			if (GetFilePath(_dirs, temp, &filePath))
			{
				objectdata.text = filePath;
			}

			nextFileIndex = dataString.find('\n', prevFileIndex);
			temp = dataString.substr(prevFileIndex, nextFileIndex - prevFileIndex);
			temp.erase(std::remove(temp.begin(), temp.end(), 13), temp.end());
			prevFileIndex = nextFileIndex + 1;
			if (GetFilePath(_dirs, temp, &filePath))
			{
				objectdata.norm = filePath;
			}

			nextFileIndex = dataString.find('\n', prevFileIndex);
			temp = dataString.substr(prevFileIndex, nextFileIndex - prevFileIndex);
			temp.erase(std::remove(temp.begin(), temp.end(), 13), temp.end());
			prevFileIndex = nextFileIndex + 1;
			if (GetFilePath(_dirs, temp, &filePath))
			{
				objectdata.spec = filePath;
			}

		}
		

		return objectdata;
	}
};

#endif
