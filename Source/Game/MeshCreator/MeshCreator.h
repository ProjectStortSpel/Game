#ifndef MESHCREATOR_H
#define MESHCREATOR_H

#include "includes.h"
#include "Tile.h"

class MeshCreator
{
	public:
		std::vector<float> position;
		std::vector<float> normals;
		std::vector<float> smoothedNormals;
		std::vector<float> tangents;
		std::vector<float> bitangents;
		std::vector<float> uvs;
		std::vector<std::string> map;

	public:
		MeshCreator();
		~MeshCreator();
		void CreateQuad();
		void CreateMesh(std::vector<std::string> map);
		void DisplaceMesh();
		void AdjacentCheck(std::vector<std::vector<Tile>> &tiles);
		void CalculateSmoothedNormals(std::vector<float> &normals, std::vector<float> &position, std::vector<float> &smoothedNormals);
		void LoadMap(std::vector<std::string>map);
		bool CompareVertex(float x, float y, float z, std::vector<int>&indexList, std::vector<float> &position);


};


#endif // !MESHCREATOR_H
