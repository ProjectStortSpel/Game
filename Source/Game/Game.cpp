#include <SDL/SDL.h>
#include <ECSL/ECSL.h>
#include <vector>
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>


int main(int argc, char** argv)
{
	SDL_Init(SDL_INIT_EVERYTHING);
	////ECSL::WorldCreator worldCreator = ECSL::WorldCreator();
	//ECSL::Parser* parser = new ECSL::Parser();
	//ECSL::Section* section = parser->ParseFile("content/components/component.cmp");
	//ECSL::ComponentTypeReader reader;
	//std::vector<ECSL::ComponentType*> componentTypes = *reader.ReadComponents("content/components/component.cmp", section);
	//delete(parser);
	//delete(section);
	////worldInitializer.Add<int>();
	////ECSL::World* world = worldCreator.InitializeWorld(100);
	ECSL::ComponentTypeManager::GetInstance().LoadComponentTypesFromDirectory("Content/components");
	SDL_Quit();
	system("pause");
	//_CrtDumpMemoryLeaks();
	return 0;
}