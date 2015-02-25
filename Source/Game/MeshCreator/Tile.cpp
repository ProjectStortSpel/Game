
#include "Tile.h"


Tile::Tile()
{
	this->depth = -4;

	this->position.x = 0;
	this->position.y = 0;

	for (int i = 0; i < directions::total; i++)
	{
		this->neighbor[i] = false;
	}

	this->ground = true;

	this->GenerateVertices();
}

Tile::Tile(int x, int y, bool ground)
{
	this->depth = -9;

	this->position.x = x;
	this->position.y = y;

	for (int i = 0; i < directions::total; i++)
	{
		this->neighbor[i] = false;		//Sätter alla grannar till false?
	}

	this->ground = ground;

	this->GenerateVertices();
}
/*
0 1 2
3 4 5
6 7 8
*/
void Tile::HaveNeighbor(directions dir)
{
	this->neighbor[dir] = true;

	
}

void Tile::GenerateVertices()
{
	//0
	this->vertices[0].x = this->position.x - 0.5f;
	this->vertices[0].y = this->position.y - 0.5f;
	this->vertices[0].z = 0;

	//1
	this->vertices[1].x = this->position.x;
	this->vertices[1].y = this->position.y - 0.5f;
	this->vertices[1].z = 0;

	//2
	this->vertices[2].x = this->position.x + 0.5f;
	this->vertices[2].y = this->position.y - 0.5f;
	this->vertices[2].z = 0;

	//3
	this->vertices[3].x = this->position.x - 0.5f;
	this->vertices[3].y = this->position.y;
	this->vertices[3].z = 0;

	//4
	this->vertices[4].x = this->position.x;
	this->vertices[4].y = this->position.y;
	this->vertices[4].z = 0;

	//5
	this->vertices[5].x = this->position.x + 0.5f;
	this->vertices[5].y = this->position.y;
	this->vertices[5].z = 0;

	//6
	this->vertices[6].x = this->position.x - 0.5f;
	this->vertices[6].y = this->position.y + 0.5f;
	this->vertices[6].z = 0;

	//7
	this->vertices[7].x = this->position.x;
	this->vertices[7].y = this->position.y + 0.5f;
	this->vertices[7].z = 0;

	//8
	this->vertices[8].x = this->position.x + 0.5f;
	this->vertices[8].y = this->position.y + 0.5f;
	this->vertices[8].z = 0;


}

void Tile::ChangeDepth(glm::vec2 centre, float longest_distance)
{
	float total = 0;
	 
	for (int i = 0; i < 9; i++)		//för varje vertris i en tile
	{
		/*vertex position relative from centre*/
		glm::vec2 vertex_position(this->vertices[i].x - centre.x, this->vertices[i].y - centre.y);

		/*calculate distance from centre*/
		float length = sqrt(vertex_position.x*vertex_position.x +vertex_position.y*vertex_position.y);

		/*Calcuate distance factor*/
		float distance_factor = length / longest_distance;

		/*Calculate the z value*/
		//this->vertices[i].z = (1 - distance_factor) * this->depth - abs(sinf(length)*this->depth / 10)*2 - abs(cosf(length)*this->depth / 10)*2;
		float inversefactor = (1 - distance_factor);
		float pow = inversefactor*inversefactor;
		float scale1 = this->depth / 5;
		float scale2 = this->depth*pow / 8;

		this->vertices[i].z = inversefactor * pow * this->depth ;
		
		if (pow < 0.6f && pow > 0.25f)
		{
			this->vertices[i].z += -abs(sinf(length*pow) * (scale2*2));
			this->vertices[i].z += -abs(cosf(length*pow) * (scale2 * 2));

			this->vertices[i].z += -abs(sinf(length) *pow);
			this->vertices[i].z += -abs(cosf(length)*pow);

			this->vertices[i].z += -abs(cosf(vertex_position.x))*pow*scale2;
			this->vertices[i].z += -abs(cosf(vertex_position.y))*pow*scale2;

			this->vertices[i].z += -abs(sinf(vertex_position.x)*scale1)*pow * scale1;
			this->vertices[i].z += -abs(sinf(vertex_position.y)*scale1)*pow * scale1;

			this->vertices[i].z += -abs(sinf(vertex_position.x)*scale2);
			this->vertices[i].z += -abs(sinf(vertex_position.y)*scale2);
		}
		if (this->vertices[i].z > 0)
		{
			this->vertices[i].z = -this->vertices[i].z;
		}
		//this->vertices[i].z = this->depth;
		total += this->vertices[i].z;

		this->FixEdges();
	}

	this->vertices[4].z = total / 9;		//mittenvertrisen får ett z-värde som är medelvärde
	if (this->vertices[4].z == 0)			//för allas z-värde
	{
		this->vertices[4].z = -0.5f;

	}
}

void Tile::FixEdges()
{
	if (!this->neighbor[directions::up])
	{
		this->vertices[0].z = 0;
		this->vertices[1].z = 0;
		this->vertices[2].z = 0;
	}
	if (!this->neighbor[directions::left])
	{
		this->vertices[0].z = 0;
		this->vertices[3].z = 0;
		this->vertices[6].z = 0;
	}

	if (!this->neighbor[directions::right])
	{
		this->vertices[2].z = 0;
		this->vertices[5].z = 0;
		this->vertices[8].z = 0;
	}

	if (!this->neighbor[directions::down])
	{
		this->vertices[6].z = 0;
		this->vertices[7].z = 0;
		this->vertices[8].z = 0;
	}

	if (!this->neighbor[directions::up_left])
	{
		this->vertices[0].z = 0;
	}

	if (!this->neighbor[directions::up_right])
	{
		this->vertices[2].z = 0;
	}

	if (!this->neighbor[directions::down_left])
	{
		this->vertices[6].z = 0;
	}

	if (!this->neighbor[directions::down_right])
	{
		this->vertices[8].z = 0;
	}
}
bool Tile::IsGround()
{
	return this->ground;
}

void Tile::CallCalculations(std::vector<float> &normals, std::vector<float> &tangents, std::vector<float> &bitangents, std::vector<float> &uvs)
{
	if (IsGround())
	{
		SetUV(uvs); //hårdkoda all the uvs

		//triangle 1
		CalculateNormals(0, 4, 3, normals, tangents, bitangents);
		//calcTangentsand bitangents anropas i calculate normals
		//triangle 2
		CalculateNormals(1, 4, 0, normals, tangents, bitangents);
		//triangle 3
		CalculateNormals(2, 4, 1, normals, tangents, bitangents);
		//triangle 4
		CalculateNormals(5, 4, 2, normals, tangents, bitangents);
		//triangle 5
		CalculateNormals(3, 4, 6, normals, tangents, bitangents);
		//triangle 6
		CalculateNormals(6, 4, 7, normals, tangents, bitangents);
		//triangle 7
		CalculateNormals(7, 4, 8, normals, tangents, bitangents);
		//triangle 8
		CalculateNormals(8, 4, 5, normals, tangents, bitangents);
	}
}

void Tile::CalculateNormals(int index1, int index2, int index3, std::vector<float> &normals, std::vector<float> &tangents, std::vector<float> &bitangents)
{
	glm::vec3 a(0);							//räknar ut en normal för en triangel och 
	glm::vec3 b(0);							//stoppar in samma normal för varje vertris 
	glm::vec3 newNormal(0);					//i listan med normaler

	a.x = vertices[index1].x - vertices[index2].x;
	a.y = vertices[index1].y - vertices[index2].y;
	a.z = vertices[index1].z - vertices[index2].z;

	b.x = vertices[index3].x - vertices[index2].x;
	b.y = vertices[index3].y - vertices[index2].y;
	b.z = vertices[index3].z - vertices[index2].z;

	newNormal = glm::cross(a,b);
	newNormal = glm::normalize(newNormal);



	for (int i = 0; i < 3; i++)					
	{
		normals.push_back(newNormal.x);
		normals.push_back(newNormal.z);
		normals.push_back(newNormal.y);
	}

	CalculateTangents(newNormal, a, tangents, bitangents);

}


void Tile::CalculateTangents(glm::vec3 normal, glm::vec3 vector, std::vector<float> &tangents, std::vector<float> &bitangents)
{
	for (int i = 0; i < 3; i++)
	{
		tangents.push_back(vector.x);
		tangents.push_back(vector.z);
		tangents.push_back(vector.y);
	}

	glm::vec3 bitangent(0);

	bitangent = glm::cross(vector, normal);

	for (int i = 0; i < 3; i++)
	{
		bitangents.push_back(bitangent.x);
		bitangents.push_back(bitangent.z);
		bitangents.push_back(bitangent.y);
	}
}

void Tile::SetUV(std::vector<float> &uvs)
{	
	
	//triangle 1
	uvs.push_back(0.0f);
	uvs.push_back(1.0f);
	uvs.push_back(0.5f);
	uvs.push_back(0.5f);
	uvs.push_back(0.0f);
	uvs.push_back(0.5f);

	

	//triangle 2
	uvs.push_back(0.5f);
	uvs.push_back(1.0f);
	uvs.push_back(0.5f);
	uvs.push_back(0.5f);
	uvs.push_back(0.0f);
	uvs.push_back(1.0f);

	

	//triangle 3
	uvs.push_back(1.0f);
	uvs.push_back(1.0f);
	uvs.push_back(0.5f);
	uvs.push_back(0.5f);
	uvs.push_back(0.5f);
	uvs.push_back(1.0f);

	

	//triangle 4
	uvs.push_back(1.0f);
	uvs.push_back(0.5f);
	uvs.push_back(0.5f);
	uvs.push_back(0.5f);
	uvs.push_back(1.0f);
	uvs.push_back(1.0f);

	

	//triangle 5
	uvs.push_back(0.0f);
	uvs.push_back(0.5f);
	uvs.push_back(0.5f);
	uvs.push_back(0.5f);
	uvs.push_back(0.0f);
	uvs.push_back(0.0f);

	

	//triangle 6
	uvs.push_back(0.0f);
	uvs.push_back(0.0f);
	uvs.push_back(0.5f);
	uvs.push_back(0.5f);
	uvs.push_back(0.5f);
	uvs.push_back(0.0f);

	

	//triangle 7
	uvs.push_back(0.5f);
	uvs.push_back(0.0f);
	uvs.push_back(0.5f);
	uvs.push_back(0.5f);
	uvs.push_back(1.0f);
	uvs.push_back(0.0f);

	

	//triangle 8
	uvs.push_back(1.0f);
	uvs.push_back(0.0f);
	uvs.push_back(0.5f);
	uvs.push_back(0.5f);
	uvs.push_back(1.0f);
	uvs.push_back(0.5f);

	


	

}

void Tile::GetTriangleVertices(std::vector<float> &vertices)
{
	if (this->ground)
	{
		//triangle 1
		this->pushToVector(vertices, 0);
		this->pushToVector(vertices, 4);
		this->pushToVector(vertices, 3);

		//triangle 2
		this->pushToVector(vertices, 1);
		this->pushToVector(vertices, 4);
		this->pushToVector(vertices, 0);

		//triangle 3
		this->pushToVector(vertices, 2);
		this->pushToVector(vertices, 4);
		this->pushToVector(vertices, 1);

		//triangle 4 
		this->pushToVector(vertices, 5);
		this->pushToVector(vertices, 4);
		this->pushToVector(vertices, 2);

		//triangle 5
		this->pushToVector(vertices, 3);
		this->pushToVector(vertices, 4);
		this->pushToVector(vertices, 6);

		//triangle 6
		this->pushToVector(vertices, 6);
		this->pushToVector(vertices, 4);
		this->pushToVector(vertices, 7);

		//triangle 7
		this->pushToVector(vertices, 7);
		this->pushToVector(vertices, 4);
		this->pushToVector(vertices, 8);

		//triangle 8 
		this->pushToVector(vertices, 8);
		this->pushToVector(vertices, 4);
		this->pushToVector(vertices, 5);
	}

}

void Tile::pushToVector(std::vector<float> &vertices, int vertex)
{
	vertices.push_back(this->vertices[vertex].x);
	vertices.push_back(this->vertices[vertex].z);
	vertices.push_back(this->vertices[vertex].y);
}

Tile::~Tile()
{
}