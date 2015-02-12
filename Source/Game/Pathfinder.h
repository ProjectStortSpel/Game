#ifndef PATHFINDER_H
#define PATHFINDER_H

#include <vector>
#include "LuaBridge/AI/LuaPotentialFieldHandler.h"

typedef LuaBridge::PotentialField::PF potential_field;

struct PotentialFieldData
{
	float				weight;
	potential_field*	potentialField;

	PotentialFieldData()
	{
		potentialField = NULL;
		weight = 0;
	}

	PotentialFieldData(potential_field* pf, int POWER)
	{
		potentialField = pf;
		weight = POWER;
	}
};

struct coord
{
	int x, y;

	coord()
	{
		this->x = 0;
		this->y = 0;
	}

	coord(int x, int y)
	{
		this->x = x;
		this->y = y;
	}
};

struct tile_data
{
	float walking_cost;
	coord position;
	bool walkable;
};

struct pathfindingnode
{
	float					g;
	float					h;
	bool					walkable;
	float					walk_cost;
	struct coord			position;
	struct pathfindingnode	*parent;
};

class Pathfinder
{
public:
	static Pathfinder* Instance();
	static void Destroy();
	~Pathfinder();

	void SetNodeData(const struct tile_data*** _data, int _x, int _y);
	std::vector<coord> GeneratePath(coord _start, coord _goal);
	void ChangeWalkable(int _x, int _y, bool _walkable);

	bool RemovePotentialField(potential_field* pPF);

	bool SetPotentialFieldWeight(potential_field* pPF, float weight);
	bool SetPotentialFieldUse(potential_field* pPF);

	void AddPotentialField(potential_field* pPF, float weight);

private:
	
	std::vector<PotentialFieldData>		m_pFToBeUsed;
	std::vector<PotentialFieldData>		m_potentialFields;
	coord								m_startCoord;
	coord								m_goalCoord;
	coord								m_mapSize;

	struct pathfindingnode**			m_mapData;

	bool InsideWorld(int _x, int _y);
	float CalcHeuristicValue(coord _fromToGoal);
	pathfindingnode BestNode(std::vector<pathfindingnode>& _set);
	void AddNeighbors(coord _position, std::vector<pathfindingnode>& _set, const std::vector<pathfindingnode>& _notToUse, pathfindingnode* _parent);
	bool InSet(const std::vector<pathfindingnode>& _set, int _x, int _y);

	Pathfinder(Pathfinder const&) = delete;
	void operator=(Pathfinder const&) = delete;

	void DeleteMap();

	Pathfinder();
	static Pathfinder* m_instance;
};



#endif //PATHFINDER_H
