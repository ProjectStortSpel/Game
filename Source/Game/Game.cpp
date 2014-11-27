#include <SDL/SDL.h>
#include <ECSL/ECSL.h>
#include <vector>
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

	delete(world);

	//ECSL::Parser* parser = new ECSL::Parser();
	//ECSL::Section* section = parser->ParseFile("content/components/component.cmp");
	//ECSL::ComponentTypeReader reader;
	//std::vector<ECSL::ComponentType*> componentTypes = *reader.ReadComponents("content/components/component.cmp", section);
	//delete(parser);
	//delete(section);
	////worldInitializer.Add<int>();

	//ECSL::ComponentTypeManager::GetInstance().AddComponentTypesFromDirectory("Content/components");
	
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