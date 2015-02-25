#ifndef VERTEX_H
#define VERTEX_H
#include "includes.h"

enum directions
{
	up,
	up_left,
	up_right,
	left,
	right,
	down,
	down_left,
	down_right,
	total
};
class Tile
{
	public:
		Tile();
		Tile(int x, int y, bool ground);
		~Tile();
		void GenerateVertices();
		void GetTriangleVertices(std::vector<float> &vertices);
		void CallCalculations(std::vector<float> &normals, std::vector<float> &tangents, std::vector<float> &bitangents, std::vector<float> &uvs);
		void CalculateNormals(int index1, int index2, int index3, std::vector<float> &normals, std::vector<float> &tangents, std::vector<float> &bitangents);
		
		void CalculateTangents(glm::vec3 normal, glm::vec3 vector, std::vector<float> &tangents, std::vector<float> &bitangents);
		void ChangeDepth(glm::vec2 centre, float longest_dist);
		void SetUV(std::vector<float> &uvs);
		void FixEdges();
		void HaveNeighbor(directions dir);
		bool IsGround();

	private:
		void pushToVector(std::vector<float> &vertices, int vertex);
		bool ground;

		glm::vec2 position;
		glm::vec3 vertices[9];

		std::vector<float> bitangents;

		bool neighbor[directions::total];
		float depth;
		
};



#endif // !VERTEX_H
