#include <algorithm>
#include "MeshCreator.h"


MeshCreator::MeshCreator()
{
}

MeshCreator::~MeshCreator()
{

}

void MeshCreator::CreateQuad()
{
	std::vector<std::vector<Tile>>tiles; //listlistan

	glm::vec2 centre(0, 0);
	int div = 0;

	for (int y = 0; y < map.size(); y++)			//skapa planet efter kartan
	{
		//stoppa in en tilelista i listlista
		tiles.push_back(std::vector<Tile>());
		for (int x = 0; x < map[0].length(); x++)
		{
			if (map[y][x] == 'o')
			{ 
				tiles[y].push_back(Tile(x,y, false));//tile i tilelista
			}

			else
			{
				tiles[y].push_back(Tile(x,y, true));
				div++;
				centre.x += x;
				centre.y += y;
			}
		}
	}

	centre.x = centre.x / div;
	centre.y = centre.y / div;


	AdjacentCheck(tiles);

	float extra_length = sqrt(0.5f*0.5f + 0.5f*0.5f);
	float longest_length = 0;

	for (int x = 0; x < map.size(); x++)
	{
		for (int y = 0; y < map[0].length(); y++)
		{
			float xpos = x - centre.x;
			float ypos = y - centre.y;



			float length = sqrt(xpos*xpos + ypos*ypos) + extra_length;
			if (longest_length < length)
				longest_length = length;
		}
	}

	for (int x = 0; x < map.size(); x++)
	{
		for (int y = 0; y < map[0].length(); y++)
		{
			tiles[x][y].ChangeDepth(centre, longest_length);
		}
	}


	for (int x = 0; x < map.size(); x++)
	{
		for (int y = 0; y < map[0].length(); y++)
		{
			tiles[x][y].GetTriangleVertices(position);
			tiles[x][y].CallCalculations(normals, tangents, bitangents, uvs);
			
		}
	}

	//-------------------------------------
	//SMOOTH NORMALS
	//-------------------------------------

	//smoothedNormals.clear();
	//CalculateSmoothedNormals(normals, position, smoothedNormals);

	//-------------------------------------
	//-------------------------------------
	
}

void MeshCreator::LoadMap(std::vector<std::string> map)
{
	this->map = map;
	/*map.clear();

	map.push_back("ooo..d.....xoo");
	map.push_back("oo...d...ooooo");
	map.push_back("ool..d.dooollo");
	map.push_back("o.u..hooooxxxo");
	map.push_back("o.u.fx.oooo..o");
	map.push_back("o.u.xx.d.....o");
	map.push_back("o.u....d...h.o");
	map.push_back("o2u....d..3.oo");
	map.push_back("oru..h.d...ooo");
	map.push_back("o.s.s.sds.sooo");*/
}

void MeshCreator::CreateMesh(std::vector<std::string> map)
{
	LoadMap(map);
	CreateQuad();
}

void MeshCreator::AdjacentCheck(std::vector<std::vector<Tile>> &tiles)
{
	//adjacentcheck
	for (int y = 0; y < map.size(); y++)
	{
		for (int x = 0; x < map[0].length(); x++)
		
		{
			if (tiles[y][x].IsGround()) // om tilen är ground så börjar vi kolla
			{
				if (x < map[0].length() - 1)
				{
					if (tiles[y][x + 1].IsGround()) //om den till höger är ground
					{
						tiles[y][x].HaveNeighbor(directions::right); //sparar att den har en granne till höger
					}
					if (y < map.size() - 1)
					{
						if (tiles[y + 1][x + 1].IsGround())
						{
							tiles[y][x].HaveNeighbor(directions::down_right);
						}
					}
					if (y > 0)
					{
						if (tiles[y - 1][x + 1].IsGround())
						{
							tiles[y][x].HaveNeighbor(directions::up_right);
						}
					}
				}
				if (x > 0)
				{
					if (tiles[y][x - 1].IsGround())
					{
						tiles[y][x].HaveNeighbor(directions::left);
					}

					if (y < map.size() - 1)
					{
						if (tiles[y + 1][x - 1].IsGround())
						{
							tiles[y][x].HaveNeighbor(directions::down_left);
						}
					}
					if (y > 0)
					{
						if (tiles[y - 1][x - 1].IsGround())
						{
							tiles[y][x].HaveNeighbor(directions::up_left);
						}
					}
				}
				if (y < map.size() - 1)
				{
					if (tiles[y + 1][x].IsGround())
					{
						tiles[y][x].HaveNeighbor(directions::down);
					}
				}
				if (y > 0)
				{
					if (tiles[y - 1][x].IsGround())
					{
						tiles[y][x].HaveNeighbor(directions::up);
					}
				}

			}
		}
	}
}

void MeshCreator::CalculateSmoothedNormals(std::vector<float> &normals, std::vector<float> &position, std::vector<float> &smoothedNormals)
{
	smoothedNormals.resize(position.size());
	std::vector<int> indexList;

	for (int i = 0; i < position.size(); i += 3) // Nya tiles med nya värden och djup!
	{
		float posX = 0.0f;
		float posY = 0.0f;
		float posZ = 0.0f;

		glm::vec3 newNormal(0);

		posX = position[i + 0];
		posY = position[i + 1];
		posZ = position[i + 2];

		indexList.clear();

		if (CompareVertex(posX, posY, posZ, indexList, position))
		{
			for (int c = 0; c < indexList.size(); c++)
			{								//hämtar xyz ur newNormals till en vektor
				glm::vec3 toNormalize(normals[indexList[c] + 0], normals[indexList[c] + 1], normals[indexList[c] + 2]);
				newNormal += glm::normalize(toNormalize);	//normaliserar och adderar den här vektorn med newNormal
			}
			newNormal = glm::normalize(newNormal);	//newNormal består av summan av de normalerna som hittades med samma xyz, den normaliseras 

			for (int d = 0; d < indexList.size(); d++)
			{
				smoothedNormals[indexList[d] + 0] = newNormal.x;
				smoothedNormals[indexList[d] + 1] = newNormal.y;
				smoothedNormals[indexList[d] + 2] = newNormal.z;
			}
			indexList.clear();
		}
	}
}

bool MeshCreator::CompareVertex(float x, float y, float z, std::vector<int> &indexList, std::vector<float> &position)
{
	bool ret = false;
	for (int t = 0; t < position.size(); t += 3)
	{
		float posX = position[t + 0];
		float posY = position[t + 1];
		float posZ = position[t + 2];

		//xdiff = absolutbeloppet av x-x
		float xdiff = glm::abs(posX - x); //skillnaden mellan x och x
		float ydiff = glm::abs(posY - y);
		float zdiff = glm::abs(posZ - z);

		if (xdiff < 0.01f && ydiff < 0.01f && zdiff < 0.01f)
		{
			indexList.push_back(t);

			ret = true;
		}
	}
	return ret;
}

