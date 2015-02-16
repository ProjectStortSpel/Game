#ifndef PATHFINDER_H
#define PATHFINDER_H

#include <vector>

typedef std::vector<std::vector<float>> potential_field;

/* Coordinates */
struct coord
{
	/* describes the x and y position */
	int x, y;
	/* Defult constructor x = 0; y = 0; */
	coord()
	{
		this->x = 0;
		this->y = 0;
	}
	/* Constructor x = x, and y = y */
	coord(int x, int y)
	{
		this->x = x;
		this->y = y;
	}
};
/* tile data */
struct tile_data
{
	/* What it cost to stand/walk on this tile */
	float walking_cost;
	/* Position in world */
	coord position;
	/* Is the tile walkable */
	bool walkable;
};
/* Data struct for the pathfinder to store data */
struct pathfindingnode
{
	/* The cost to walk to this position from the start node. */
	float					g;
	/* The estamated cost to the goal node from this one. */
	float					h;
	/* If this node is walkable or not. */
	bool					walkable;
	/* The cost to cross/stand on this node. */
	float					walk_cost;
	/* The position of the node in the world. */
	struct coord			position;
	/* Which node did the pathfinder visit before this one. */
	struct pathfindingnode	*parent;
};

class Pathfinder
{
public:
	/*
		Instance				: Get a instance which you can use
		return					: returns the instance of the singleton
	*/
	static Pathfinder* Instance();
	/*
		Destroy					: Destroys the class, free the alocated space.
	*/
	static void Destroy();
	/*
		DESTRUCTOR
	*/
	~Pathfinder();
	/*
		SetNodeData				: Informs the pathfinder about the world.
		_data					: Data describing the world.
		_x						: The x size of the world.
		_y						: The y size of the world.
	*/
	void SetNodeData(const struct tile_data*** _data, int _x, int _y);
	
	/*
		ChangeWalkable			: Change if a tile is walkable or not.
		_x						: The tiles x position.
		_y						: The tiles y position.
		_walkable				: returns if the tile is walkable or not.
	*/
	void ChangeWalkable(int _x, int _y, bool _walkable);
	/*
		GeneratePath			: Generates the path from a posision to another
		_start					: Path should be genereted from this position.
		_goal					: Path should be genereted to this position.
		return					: Returns the vector of coords, of which describes the path.
	*/
	std::vector<coord> GeneratePath(coord _start, coord _goal, float &_totalCost);	

	/*
		RemovePotentialField	: Remove a potential field from the pathfinder
		_pPF					: The pointer to the potential field which should be removed.
		return					: Returns if the function was able to find and remove the _pPF.
	*/
	bool RemovePotentialField();
	/*
		SetPotentialFieldUse	: Tells the pathfinder to use a given potential field from now on.
		_pPF					: The pointer to the potential field.
		return					: Returns if the function was able to find and set the _pPF to be used.
	*/
	bool UsePotentialField(potential_field* _pPF);
	/*
		ClearPotentialField		: Tells the pathfinder not to use any potential fields from now on.
	*/
	void ClearPotentialField();

private:
	/*
		Contains the potential fields that will be used to genereate the path
	*/
	potential_field*					m_potentialField;
	/*
		The target which the pathfinder trying to generate a path to.
	*/
	coord								m_goalCoord;
	/*
		The size of the world of which the pathfinder is using.
	*/
	coord								m_mapSize;
	/*
		A 2 dimentional array, that contains the data form the world.
	*/
	struct pathfindingnode**			m_mapData;
	/*
	*/
	bool InSet(const std::vector<pathfindingnode>& _set, int _x, int _y);
	/*
	*/
	bool InsideWorld(int _x, int _y);
	/*
	*/
	float CalcHeuristicValue(coord _fromToGoal);
	/*
	*/
	pathfindingnode BestNode(std::vector<pathfindingnode>& _set);
	/*
	*/
	void AddNeighbors(coord _position, std::vector<pathfindingnode>& _set, const std::vector<pathfindingnode>& _notToUse, pathfindingnode* _parent);
	/*
	*/
	Pathfinder(Pathfinder const&) = delete;
	/*
	*/
	void operator=(Pathfinder const&) = delete;
	/*
	*/
	void DeleteMap();
	/*
	*/
	Pathfinder();
	/*
	*/
	static Pathfinder* m_instance;
};



#endif //PATHFINDER_H
