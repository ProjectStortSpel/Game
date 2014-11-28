#include <SDL/SDL.h>
#include <ECSL/ECSL.h>
#include <vector>
#include <time.h>
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

using namespace ECSL;

class TestSystem : public ECSL::System
{
public:
	TestSystem() { }
	~TestSystem() { }

	void Run()
	{
		printf("Testsystem run()\n");
	}
	void Initialize()
	{
		AddComponentTypeToFilter("Position", ECSL::ComponentFilter::Mandatory);
		AddComponentTypeToFilter("Position", ECSL::ComponentFilter::RequiresOneOf);
		AddComponentTypeToFilter("Velocity", ECSL::ComponentFilter::RequiresOneOf);
		AddComponentTypeToFilter("Position", ECSL::ComponentFilter::Excluded);

		printf("Testsystem Initialize()\n");
	}

	void OnEntityAdded(unsigned int _entityId)
	{
		printf("Testsystem OnEntityAdded()\n");
	}
	void OnEntityRemoved(unsigned int _entityId)
	{
		printf("Testsystem OnEntityRemoved()\n");
	}
private:
};

void lol()
{
	ComponentTypeManager::GetInstance().LoadComponentTypesFromDirectory("Content/components");
	ECSL::WorldCreator worldCreator = ECSL::WorldCreator();
	//worldCreator.AddSystemGroup();
	//worldCreator.AddSystemToCurrentGroup<TestSystem>();
	//worldCreator.AddComponentType("Position");
	worldCreator.AddComponentType("Velocity");
	ECSL::World* world = worldCreator.CreateWorld(100);

	//int id = world->CreateNewEntity();
	//world->CreateComponentAndAddTo("Velocity", id);
	//world->CreateComponentAndAddTo("Position", id);

	//world->KillEntity(id);

	delete(world);
	delete(&ComponentTypeManager::GetInstance());
	delete(&BitSet::BitSetConverter::GetInstance());
}

int main(int argc, char** argv)
{
	SDL_Init(SDL_INIT_EVERYTHING);
	lol();
	SDL_Quit();
	system("pause");
	_CrtDumpMemoryLeaks();
	return 0;
}