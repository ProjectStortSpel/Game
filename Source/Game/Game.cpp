#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#ifdef _DEBUG
#ifndef DBG_NEW
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define new DBG_NEW
#endif
#endif 

#include <SDL/SDL.h>
#include <ECSL/ECSL.h>
#include <vector>
#include <time.h>


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
		AddComponentTypeToFilter("TestComponenta", ECSL::ComponentFilter::Mandatory);
		AddComponentTypeToFilter("TestComponentb", ECSL::ComponentFilter::Mandatory);
		AddComponentTypeToFilter("TestComponentc", ECSL::ComponentFilter::Mandatory);
		AddComponentTypeToFilter("TestComponentd", ECSL::ComponentFilter::Mandatory);

		AddComponentTypeToFilter("TestComponente", ECSL::ComponentFilter::Excluded);
		AddComponentTypeToFilter("TestComponentf", ECSL::ComponentFilter::Excluded);
		AddComponentTypeToFilter("TestComponentg", ECSL::ComponentFilter::Excluded);
		AddComponentTypeToFilter("TestComponenth", ECSL::ComponentFilter::Excluded);

		AddComponentTypeToFilter("TestComponenti", ECSL::ComponentFilter::RequiresOneOf);
		AddComponentTypeToFilter("TestComponentj", ECSL::ComponentFilter::RequiresOneOf);
		AddComponentTypeToFilter("TestComponentk", ECSL::ComponentFilter::RequiresOneOf);
		AddComponentTypeToFilter("TestComponentl", ECSL::ComponentFilter::RequiresOneOf);

		SystemBitmask* test = GetSystemBitmask(ECSL::ComponentFilter::Mandatory);

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
	ECSL::WorldCreator worldCreator = ECSL::WorldCreator();

	worldCreator.AddSystemGroup();
	worldCreator.AddSystemToCurrentGroup<TestSystem>();

	ECSL::World* world = worldCreator.InitializeWorld(100);

	delete world;

	ECSL::DataMap* test = new ECSL::DataMap(24);
	delete test;
}

int main(int argc, char** argv)
{
	SDL_Init(SDL_INIT_EVERYTHING);
	lol();




	delete(&ComponentTypeManager::GetInstance());
	delete(&BitSet::BitSetConverter::GetInstance());
	SDL_Quit();
	//system("pause");

	_CrtDumpMemoryLeaks();
	return 0;
}