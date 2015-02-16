#include "Pathfinder.h"

#include <cstddef>

Pathfinder* Pathfinder::m_instance = 0;

Pathfinder* Pathfinder::Instance()
{
	if (!Pathfinder::m_instance)
	{
		Pathfinder::m_instance = new Pathfinder();
	}

	return Pathfinder::m_instance;
}

void Pathfinder::Destroy()
{
	if (Pathfinder::m_instance)
		delete Pathfinder::m_instance;
}

Pathfinder::Pathfinder()
{
	this->m_potentialField = NULL;
	this->m_mapSize = coord(0, 0);
	this->m_mapData = NULL;
}

void Pathfinder::SetNodeData(const struct tile_data*** _data, int _x, int _y)
{
	const struct tile_data **use = (*_data);
	this->DeleteMap();
	this->m_mapSize = coord(_x, _y);

	
	// allocationg space
	this->m_mapData = new struct pathfindingnode*[_x];
	for (int i = 0; i < _x; ++i)
	{
		this->m_mapData[i] = new struct pathfindingnode[_y];
	}

	for (int i = 0; i < _y; ++i)
	{
		for (int j = 0; j < _x; ++j)
		{
			this->m_mapData[j][i].walkable = use[j][i].walkable;
			this->m_mapData[j][i].position = coord(j,i);
			this->m_mapData[j][i].walk_cost = use[j][i].walking_cost;
			this->m_mapData[j][i].parent = 0;
		}
	}
}

void Pathfinder::ChangeWalkable(int _x, int _y, bool _walkable)
{
	//printf("Got data form LuaEmbedder: (%i,%i), %s", _x, _y, _walkable ? "True" : "False");
	if (this->InsideWorld(_x, _y))
	{
		if (this->m_mapData)
		{
			this->m_mapData[_x][_y].walkable = _walkable;
		}
	}
}

bool Pathfinder::RemovePotentialField()
{
	this->m_potentialField = NULL;
	return false;
}
bool Pathfinder::UsePotentialField(potential_field* pPF)
{
	if (pPF)
	{
		this->m_potentialField = pPF;
	}
	return false;
}

void Pathfinder::ClearPotentialField()
{
	this->m_potentialField = NULL;
}

float Pathfinder::CalcHeuristicValue(coord _fromGoal)
{
	float ret_value;

	float dist_x = _fromGoal.x - this->m_goalCoord.x;
	float dist_y = _fromGoal.y - this->m_goalCoord.y;

	ret_value = (dist_x + dist_y);

	return ret_value;
}

void Pathfinder::AddNeighbors(coord _position, std::vector<pathfindingnode>& _set, const std::vector<pathfindingnode>& _notToUse, pathfindingnode* _parent)
{
	int x = _position.x + 1;
	int y = _position.y;

	if (this->InsideWorld(x, y) && this->m_mapData[x][y].walkable && !this->InSet(_notToUse, x, y))
	{
		float cost = this->m_mapData[x][y].walk_cost;
		if (!this->InSet(_set, x, y))
		{
			if (this->m_potentialField)
			{
				float value = (*this->m_potentialField)[x][y];
				cost += value;
				//printf("%i ", (int)cost);
			}
			this->m_mapData[x][y].parent = _parent;
			this->m_mapData[x][y].g = _parent->g + cost;
			//printf("%.1f ", (this->m_mapData[x][y].walk_cost));
			//printf("%.1f ", (this->m_mapData[x][y].walk_cost));
			
			if (y != _parent->parent->position.y)
			{
				this->m_mapData[x][y].g += cost;
			}

			this->m_mapData[x][y].h = this->CalcHeuristicValue(coord(x, y));
			_set.push_back(this->m_mapData[x][y]);
		}
		else if (y == _parent->parent->position.y && this->m_mapData[x][y].g > _parent->g + this->m_mapData[x][y].walk_cost)
		{
			this->m_mapData[x][y].parent = _parent;
			this->m_mapData[x][y].g = _parent->g + this->m_mapData[x][y].walk_cost;

		}
		else if (y != _parent->parent->position.y && this->m_mapData[x][y].g > _parent->g + this->m_mapData[x][y].walk_cost + cost)
		{
			this->m_mapData[x][y].parent = _parent;
			this->m_mapData[x][y].g = _parent->g + this->m_mapData[x][y].walk_cost + cost;
		}
	}

	x = _position.x - 1;
	y = _position.y;

	if (this->InsideWorld(x, y) && this->m_mapData[x][y].walkable && !this->InSet(_notToUse, x, y))
	{
		float cost = this->m_mapData[x][y].walk_cost;
		if (!this->InSet(_set, x, y))
		{
			
			if (this->m_potentialField)
			{
				float value = (*this->m_potentialField)[x][y];
				cost += value;
			}
			this->m_mapData[x][y].parent = _parent;
			this->m_mapData[x][y].g = _parent->g + cost;

			if (y != _parent->parent->position.y)
			{
				this->m_mapData[x][y].g += cost;
			}

			this->m_mapData[x][y].h = this->CalcHeuristicValue(coord(x, y));
			_set.push_back(this->m_mapData[x][y]);
		}
		else if (y == _parent->parent->position.y && this->m_mapData[x][y].g > _parent->g + this->m_mapData[x][y].walk_cost)
		{
			this->m_mapData[x][y].parent = _parent;
			this->m_mapData[x][y].g = _parent->g + this->m_mapData[x][y].walk_cost;

		}
		else if (y != _parent->parent->position.y && this->m_mapData[x][y].g > _parent->g + this->m_mapData[x][y].walk_cost + cost)
		{
			this->m_mapData[x][y].parent = _parent;
			this->m_mapData[x][y].g = _parent->g + this->m_mapData[x][y].walk_cost + cost;
		}
	}
	
	x = _position.x;
	y = _position.y + 1;

	if (this->InsideWorld(x, y) && this->m_mapData[x][y].walkable && !this->InSet(_notToUse, x, y))
	{
		float cost = this->m_mapData[x][y].walk_cost;
		if (!this->InSet(_set, x, y))
		{
			if (this->m_potentialField)
			{
				float value = (*this->m_potentialField)[x][y];
				cost += value;
			}
			this->m_mapData[x][y].parent = _parent;
			this->m_mapData[x][y].g = _parent->g + cost;

			if (x != _parent->parent->position.x)
			{
				this->m_mapData[x][y].g += cost;
			}

			this->m_mapData[x][y].h = this->CalcHeuristicValue(coord(x, y));
			_set.push_back(this->m_mapData[x][y]);
		}
		else if (x == _parent->parent->position.x && this->m_mapData[x][y].g > _parent->g + this->m_mapData[x][y].walk_cost)
		{
			this->m_mapData[x][y].parent = _parent;
			this->m_mapData[x][y].g = _parent->g + this->m_mapData[x][y].walk_cost;

		}
		else if (x != _parent->parent->position.x && this->m_mapData[x][y].g > _parent->g + this->m_mapData[x][y].walk_cost + cost)
		{
			this->m_mapData[x][y].parent = _parent;
			this->m_mapData[x][y].g = _parent->g + this->m_mapData[x][y].walk_cost + cost;
		}
	}

	x = _position.x;
	y = _position.y - 1;

	if (this->InsideWorld(x, y) && this->m_mapData[x][y].walkable && !this->InSet(_notToUse, x, y))
	{
		float cost = this->m_mapData[x][y].walk_cost;
		if (!this->InSet(_set, x, y))
		{
			
			if (this->m_potentialField)
			{
				float value = (*this->m_potentialField)[x][y];
				cost += value;
			}
			this->m_mapData[x][y].parent = _parent;
			this->m_mapData[x][y].g = _parent->g + cost;

			if (x != _parent->parent->position.x)
			{
				this->m_mapData[x][y].g += cost;
			}

			this->m_mapData[x][y].h = this->CalcHeuristicValue(coord(x, y));
			_set.push_back(this->m_mapData[x][y]);
		}
		else if (x == _parent->parent->position.x && this->m_mapData[x][y].g > _parent->g + this->m_mapData[x][y].walk_cost)
		{
			this->m_mapData[x][y].parent = _parent;
			this->m_mapData[x][y].g = _parent->g + this->m_mapData[x][y].walk_cost;

		}
		else if (x != _parent->parent->position.x && this->m_mapData[x][y].g > _parent->g + this->m_mapData[x][y].walk_cost + cost)
		{
			this->m_mapData[x][y].parent = _parent;
			this->m_mapData[x][y].g = _parent->g + this->m_mapData[x][y].walk_cost + cost;
		}
	}
}

bool Pathfinder::InSet(const std::vector<pathfindingnode>& _set, int _x, int _y)
{
	for (unsigned int i = 0; i < _set.size(); i++){
		if (_set[i].position.x == _x && _set[i].position.y == _y){
			return true;
		}
	}
	return false;
}

bool Pathfinder::InsideWorld(int _x, int _y)
{
	return (_x >= 0 && _x < this->m_mapSize.x &&
		_y >= 0 && _y < this->m_mapSize.y);
}

pathfindingnode Pathfinder::BestNode(std::vector<pathfindingnode>& _set)
{
	pathfindingnode ret_value;

	int j = 0;
	float best_dist = _set[0].g + _set[0].h;
	for (int i = 1; i < _set.size(); i++)
	{
		float temp_dist = _set[i].g + _set[i].h;
		if (temp_dist < best_dist)
		{
			best_dist = temp_dist;
			j = i;
		}
	}

	ret_value = _set[j];
	_set.erase(_set.begin() + j);
	return ret_value;
}

std::vector<coord> Pathfinder::GeneratePath(coord start, coord goal, float &_totalCost)
{
	std::vector<coord> ret_value;
	std::vector<pathfindingnode> open;
	std::vector<pathfindingnode> closed;


	//for (int y = 0; y < this->m_mapSize.y; y++)
	//{
	//	for (int x = 0; x < this->m_mapSize.x; x++)
	//	{
	//		printf("%.1f ", this->m_mapData[x][y].walk_cost);
	//	}
	//	printf("\n");
	//}


	this->m_goalCoord = goal;
	if (this->InsideWorld(goal.x, goal.y) && this->InsideWorld(start.x, start.y))
	{
		if (this->m_mapData[start.x][start.y].walkable && this->m_mapData[goal.x][goal.y].walkable)
		{
			this->m_mapData[start.x][start.y].parent = &this->m_mapData[start.x][start.y];
			this->m_mapData[start.x][start.y].g = 0;
			this->m_mapData[start.x][start.y].h = this->CalcHeuristicValue(start);

			open.push_back(this->m_mapData[start.x][start.y]);

			while (open.size() != 0)
			{
				pathfindingnode current_node = this->BestNode(open);
				if (current_node.position.x == goal.x && current_node.position.y == goal.y)
				{
					/*----------------------HERE-------------------*//*----------------------HERE-------------------*//*----------------------HERE-------------------*/
					/*----------------------HERE-------------------*/		_totalCost = current_node.g;			  /*----------------------HERE-------------------*/
					/*----------------------HERE-------------------*//*----------------------HERE-------------------*//*----------------------HERE-------------------*/

					coord rdy_to_push(current_node.position.x, current_node.position.y);
					ret_value.push_back(rdy_to_push);
					bool check = !(current_node.position.x == current_node.parent->position.x &&
						current_node.position.y == current_node.parent->position.y);
					while (check)
					{
						current_node = *current_node.parent;
						check = !(current_node.position.x == start.x &&
							current_node.position.y == start.y);
						coord push_me_now(current_node.position.x, current_node.position.y);
						ret_value.push_back(push_me_now);
					}
					return ret_value;
				}
				closed.push_back(current_node);
				this->AddNeighbors(current_node.position, open, closed, &m_mapData[current_node.position.x][current_node.position.y]);
			}

		}
	}
	
	return ret_value;
}

void Pathfinder::DeleteMap()
{
	for (int i = 0; i < this->m_mapSize.x; ++i)
	{
		if (this->m_mapData[i])
		{
			delete(this->m_mapData[i]);
			this->m_mapData[i] = NULL;
		}
	}
	if (this->m_mapData)
	{
		delete(this->m_mapData);
		this->m_mapData = NULL;
	}
}

Pathfinder::~Pathfinder()
{
	this->DeleteMap();
}
