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
	this->m_mapSize = coord(0, 0);
	this->m_turningCost = 0;
	this->m_mapData = NULL;
}

void Pathfinder::SetTurningCost(float _turing_cost)
{
	this->m_turningCost = _turing_cost;
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

	for (int i = 0; i < _x; ++i)
	{
		for (int j = 0; j < _y; ++j)
		{
			this->m_mapData[i][j].walkable = use[i][j].walkable;
			this->m_mapData[i][j].position = coord(i,j);
			this->m_mapData[i][j].walk_cost = use[i][j].walking_cost;
			this->m_mapData[i][j].parent = 0;
		}
	}
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
		if (!this->InSet(_set, x, y))
		{
			this->m_mapData[x][y].parent = _parent;
			this->m_mapData[x][y].g = _parent->g + this->m_mapData[x][y].walk_cost;

			if (y != _parent->parent->position.y)
			{
				this->m_mapData[x][y].g += m_turningCost;
			}

			this->m_mapData[x][y].h = this->CalcHeuristicValue(coord(x, y));
			_set.push_back(this->m_mapData[x][y]);
		}
		else if (y == _parent->parent->position.y && this->m_mapData[x][y].g > _parent->g + this->m_mapData[x][y].walk_cost)
		{
			this->m_mapData[x][y].parent = _parent;
			this->m_mapData[x][y].g = _parent->g + this->m_mapData[x][y].walk_cost;

		}
		else if (y != _parent->parent->position.y && this->m_mapData[x][y].g > _parent->g + this->m_mapData[x][y].walk_cost + m_turningCost)
		{
			this->m_mapData[x][y].parent = _parent;
			this->m_mapData[x][y].g = _parent->g + this->m_mapData[x][y].walk_cost + m_turningCost;
		}
	}

	x = _position.x - 1;
	y = _position.y;

	if (this->InsideWorld(x, y) && this->m_mapData[x][y].walkable && !this->InSet(_notToUse, x, y))
	{
		if (!this->InSet(_set, x, y))
		{
			this->m_mapData[x][y].parent = _parent;
			this->m_mapData[x][y].g = _parent->g + this->m_mapData[x][y].walk_cost;

			if (y != _parent->parent->position.y)
			{
				this->m_mapData[x][y].g += m_turningCost;
			}

			this->m_mapData[x][y].h = this->CalcHeuristicValue(coord(x, y));
			_set.push_back(this->m_mapData[x][y]);
		}
		else if (y == _parent->parent->position.y && this->m_mapData[x][y].g > _parent->g + this->m_mapData[x][y].walk_cost)
		{
			this->m_mapData[x][y].parent = _parent;
			this->m_mapData[x][y].g = _parent->g + this->m_mapData[x][y].walk_cost;

		}
		else if (y != _parent->parent->position.y && this->m_mapData[x][y].g > _parent->g + this->m_mapData[x][y].walk_cost + m_turningCost)
		{
			this->m_mapData[x][y].parent = _parent;
			this->m_mapData[x][y].g = _parent->g + this->m_mapData[x][y].walk_cost + m_turningCost;
		}
	}
	
	x = _position.x;
	y = _position.y + 1;

	if (this->InsideWorld(x, y) && this->m_mapData[x][y].walkable && !this->InSet(_notToUse, x, y))
	{
		if (!this->InSet(_set, x, y))
		{
			this->m_mapData[x][y].parent = _parent;
			this->m_mapData[x][y].g = _parent->g + this->m_mapData[x][y].walk_cost;

			if (x != _parent->parent->position.x)
			{
				this->m_mapData[x][y].g += m_turningCost;
			}

			this->m_mapData[x][y].h = this->CalcHeuristicValue(coord(x, y));
			_set.push_back(this->m_mapData[x][y]);
		}
		else if (x == _parent->parent->position.x && this->m_mapData[x][y].g > _parent->g + this->m_mapData[x][y].walk_cost)
		{
			this->m_mapData[x][y].parent = _parent;
			this->m_mapData[x][y].g = _parent->g + this->m_mapData[x][y].walk_cost;

		}
		else if (x != _parent->parent->position.x && this->m_mapData[x][y].g > _parent->g + this->m_mapData[x][y].walk_cost + m_turningCost)
		{
			this->m_mapData[x][y].parent = _parent;
			this->m_mapData[x][y].g = _parent->g + this->m_mapData[x][y].walk_cost + m_turningCost;
		}
	}

	x = _position.x;
	y = _position.y - 1;

	if (this->InsideWorld(x, y) && this->m_mapData[x][y].walkable && !this->InSet(_notToUse, x, y))
	{
		if (!this->InSet(_set, x, y))
		{
			this->m_mapData[x][y].parent = _parent;
			this->m_mapData[x][y].g = _parent->g + this->m_mapData[x][y].walk_cost;

			if (x != _parent->parent->position.x)
			{
				this->m_mapData[x][y].g += m_turningCost;
			}

			this->m_mapData[x][y].h = this->CalcHeuristicValue(coord(x, y));
			_set.push_back(this->m_mapData[x][y]);
		}
		else if (x == _parent->parent->position.x && this->m_mapData[x][y].g > _parent->g + this->m_mapData[x][y].walk_cost)
		{
			this->m_mapData[x][y].parent = _parent;
			this->m_mapData[x][y].g = _parent->g + this->m_mapData[x][y].walk_cost;

		}
		else if (x != _parent->parent->position.x && this->m_mapData[x][y].g > _parent->g + this->m_mapData[x][y].walk_cost + m_turningCost)
		{
			this->m_mapData[x][y].parent = _parent;
			this->m_mapData[x][y].g = _parent->g + this->m_mapData[x][y].walk_cost + m_turningCost;
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

std::vector<coord> Pathfinder::GeneratePath( coord start, coord goal )
{
	std::vector<coord> ret_value;
	std::vector<pathfindingnode> open;
	std::vector<pathfindingnode> closed;
	this->m_startCoord = start;
	this->m_goalCoord = goal;
	if (this->m_mapData[start.x][start.y].walkable && this->InsideWorld(start.x, start.y) && this->m_mapData[goal.x][goal.y].walkable && this->InsideWorld(goal.x, goal.y))
	{
		this->m_mapData[start.x][start.y].parent = &this->m_mapData[start.x][start.y];
		this->m_mapData[start.x][start.y].g = 0;
		this->m_mapData[start.x][start.y].h = this->CalcHeuristicValue(this->m_startCoord);

		open.push_back(this->m_mapData[start.x][start.y]);

		while (open.size() != 0)
		{
			pathfindingnode current_node = this->BestNode(open);
			if (current_node.position.x == goal.x && current_node.position.y == goal.y)
			{
				coord rdy_to_push(current_node.position.x, current_node.position.y);
				ret_value.push_back(rdy_to_push);
				bool check = !(current_node.position.x == current_node.parent->position.x &&
					current_node.position.y == current_node.parent->position.y);
				while (check)
				{
					current_node = *current_node.parent;
					check = !(current_node.position.x == this->m_startCoord.x &&
						current_node.position.y == this->m_startCoord.y);
					coord push_me_now(current_node.position.x, current_node.position.y);
					ret_value.push_back(push_me_now);
				}
				return ret_value;
			}
			closed.push_back(current_node);
			this->AddNeighbors(current_node.position, open, closed, &m_mapData[current_node.position.x][current_node.position.y]);
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
