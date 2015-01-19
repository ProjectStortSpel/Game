#include <SDL/SDL.h>
#include <ECSL/ECSL.h>
#include <MPL/MPL.h>
#include <functional>
#include <deque>
#include <atomic>
#include "Input/InputWrapper.h"
#include "Renderer/GraphicDevice.h"
#include "Timer.h"

#ifdef WIN32
	#define _CRTDBG_MAP_ALLOC
	#include <stdlib.h>
	#include <crtdbg.h>
	#include <ctime>
	#include <chrono>
#endif

#define SDL_THREAD_WINDOWS 0

using namespace ECSL;

mat4 mat[1000];

class TestSystem : public ECSL::System
{
public:
	TestSystem() { }
	~TestSystem() { }

	void Update(const RuntimeInfo& _runtime)
	{

		for (unsigned int i = 0; i < 10; ++i)
		{
			mat4* test = new mat4[1000];
			delete(test);
		}

		//printf("Testsystem run()\n");
	}
	void Initialize()
	{
		SetSystemName("TestSystem");
		AddComponentTypeToFilter("Velocity", ECSL::FilterType::Mandatory);
		AddComponentTypeToFilter("Position", ECSL::FilterType::Mandatory);
		//AddComponentTypeToFilter("Velocity", ECSL::FilterType::RequiresOneOf);
		//AddComponentTypeToFilter("Velocity", ECSL::ComponentFilter::RequiresOneOf);
		//AddComponentTypeToFilter("Position", ECSL::ComponentFilter::Excluded);

		SetUpdateTaskCount(16);
		SetEntitiesAddedTaskCount(1);
		SetEntitiesRemovedTaskCount(1);
		SetMessagesRecievedTaskCount(1);
		SubscribeTo("TestSystem2", 0);

		//float* x = (float*)GetComponent()

		printf("Testsystem Initialize()\n");
	}

	void EntitiesAdded(const RuntimeInfo& _runtime, const std::vector<unsigned int>& _entities)
	{
		//printf("Testsystem OnEntityAdded()\n");
	}
	void EntitiesRemoved(const RuntimeInfo& _runtime, const std::vector<unsigned int>& _entities)
	{
		//printf("Testsystem OnEntityRemoved()\n");
	}

	void MessagesRecieved(const RuntimeInfo& _runtime, const std::vector<Message*>& _messages)
	{
		printf("Messages recieved!");
	}
private:
};
class TestSystem2 : public ECSL::System
{
public:
	TestSystem2() { }
	~TestSystem2() { }

	void Update(const RuntimeInfo& _runtime)
	{
		//printf("Testsystem2 run()\n");
	}
	void Initialize()
	{
		SetSystemName("TestSystem2");
		AddComponentTypeToFilter("Velocity", ECSL::FilterType::Mandatory);
		//AddComponentTypeToFilter("Velocity", ECSL::FilterType::RequiresOneOf);
		//AddComponentTypeToFilter("Velocity", ECSL::ComponentFilter::RequiresOneOf);
		//AddComponentTypeToFilter("Position", ECSL::ComponentFilter::Excluded);
		//SetUpdateTaskCount(4);

		printf("Testsystem2 Initialize()\n");
	}

	void OnEntityAdded(unsigned int _entityId)
	{
		printf("Testsystem2 OnEntityAdded()\n");
	}
	void OnEntityRemoved(unsigned int _entityId)
	{
		printf("Testsystem2 OnEntityRemoved()\n");
	}
private:
};


class Test
{
public:
	float A;
	float B;
	std::string abc;
};

class Test2 : public Test
{
public:
	std::string abcd;
	float agsdg;
	float srhijdfh;
	float sdfiohj;
};

std::unordered_map<unsigned int, std::vector<unsigned int>*>* map;
std::vector<std::vector<unsigned int>*>* vec;

bool* boolTest;

void lol()
{
	//ComponentTypeManager::GetInstance().LoadComponentTypesFromDirectory("content/components");
	//ECSL::WorldCreator worldCreator = ECSL::WorldCreator();
	//worldCreator.AddSystemGroup();
	//worldCreator.AddSystemToCurrentGroup<TestSystem>();
	//worldCreator.AddSystemToCurrentGroup<TestSystem2>();
	////worldCreator.AddLuaSystemToCurrentGroup(new TestSystem());
	//auto componentTypes = ComponentTypeManager::GetInstance().GetComponentTypes();
	//for (auto it = componentTypes->begin(); it != componentTypes->end(); ++it)
	//	worldCreator.AddComponentType(it->second->GetName());
	//ECSL::World* world = worldCreator.CreateWorld(100);

	//int id = world->CreateNewEntity();
	//world->CreateComponentAndAddTo("Velocity", id);
	////world->CreateComponentAndAddTo("Position", id);
	////world->RemoveComponentFrom("Position", id);
	////world->CreateComponentAndAddTo("Position", id);

	////world->KillEntity(id);

	//world->Update(0.01f);

	////world->KillEntity(id);
	//world->CreateComponentAndAddTo("Position", id);

	//world->Update(0.01f);

	//world->RemoveComponentFrom("Position", id);

	//world->Update(0.01f);

	map = new std::unordered_map<unsigned int, std::vector<unsigned int>*>();
	vec = new std::vector<std::vector<unsigned int>*>();
	srand(time(NULL));

	for (unsigned int i = 0; i < 1000; ++i)
	{
		for (unsigned int n = 0; n < 4; ++n)
		{
			if (n == 0)
			{
				(*map)[i] = new std::vector<unsigned int>();
				(*map)[i]->push_back(i);
				vec->push_back(new std::vector<unsigned int>());
				(*vec)[i]->push_back(i);
			}
			else
				(*vec)[i]->push_back(0);
		}
	}

	boolTest = new bool[500];

	boolTest[50] = true;
	boolTest[120] = true;
	boolTest[230] = true;
	boolTest[430] = true;

	unsigned int num = 1000;

	unsigned int sumValue1 = 0;
	unsigned int sumValue2 = 0;
	unsigned int sumValue3 = 0;
	std::chrono::time_point<std::chrono::system_clock> start1, end1, start2, end2, start3, end3;

	start1 = std::chrono::system_clock::now();
	for (unsigned int i = 0; i < num; ++i)
	{
		for (auto value : *map)
		{
			for (auto second : *value.second)
				sumValue1 += second;
		}
	}
	end1 = std::chrono::system_clock::now();

	start2 = std::chrono::system_clock::now();
	for (unsigned int i = 0; i < num; ++i)
	{
		for (auto value : *vec)
		{
			for (auto second : *value)
				sumValue2 += second;
		}
	}
	end2 = std::chrono::system_clock::now();

	start3 = std::chrono::system_clock::now();

	end3 = std::chrono::system_clock::now();

	std::chrono::duration<double> elapsed_seconds = end1 - start1;
	printf("Time: %f\n", elapsed_seconds);
	elapsed_seconds = end2 - start2;
	printf("Time: %f\n", elapsed_seconds);
	elapsed_seconds = end3 - start3;
	printf("Time: %f\n", elapsed_seconds);

	printf("Sums:\n%i\n%i\n%i\n", sumValue1, sumValue2, sumValue3);
}
void LoadAlotOfBoxes(Renderer::GraphicDevice* r)
{
	// ADDING TEMP OBJECTS
	for (int x = 0; x < 10; x++)
	{
		for (int y = 0; y < 10; y++)
		{
			mat[x + y * 10] = glm::translate(vec3(x - 5, -1, y - 5));
			r->LoadModel("content/models/cube/", "cube.object", &mat[x + y * 10]);
		}
	}
}

void Start()
{
	/*	Initialize Renderer and Input	*/
	Renderer::GraphicDevice RENDERER = Renderer::GraphicDevice();
	RENDERER.Init();

	Input::InputWrapper* INPUT = &Input::InputWrapper::GetInstance();

	LoadAlotOfBoxes(&RENDERER);
	mat[100] = glm::translate(vec3(0, 0, 0));
	int modelid = RENDERER.LoadModel("content/models/cube/", "cube.object", &mat[100]); // LOADMODEL RETURNS THE MODELID
	RENDERER.ChangeModelTexture(modelid, "content/models/cube/NM_tst.png"); // CHANGING TEXTURE ON MODELID

	MPL::TaskManager::GetInstance().CreateSlaves();

	ComponentTypeManager::GetInstance().LoadComponentTypesFromDirectory("content/components");
	ECSL::WorldCreator worldCreator = ECSL::WorldCreator();
	worldCreator.AddSystemGroup();
	worldCreator.AddSystemToCurrentGroup<TestSystem>();
	worldCreator.AddSystemToCurrentGroup<TestSystem2>();
	//for (unsigned int i = 0; i < 3; ++i)
	//{
	//	worldCreator.AddSystemGroup();
	//	for (int j = 0; j < 5; ++j)
	//		worldCreator.AddLuaSystemToCurrentGroup(new TestSystem());
	//}
	//worldCreator.AddLuaSystemToCurrentGroup(new TestSystem());
	auto componentTypes = ComponentTypeManager::GetInstance().GetComponentTypes();
	for (auto it = componentTypes->begin(); it != componentTypes->end(); ++it)
		worldCreator.AddComponentType(it->second->GetName());
	ECSL::World* world = worldCreator.CreateWorld(1000);

	bool lol = true;
	float cd = 1.0f;
	Timer timer;
	while (lol)
	{
		// DT COUNTER
		float dt = timer.ElapsedTimeInSeconds();
		timer.Reset();

		INPUT->Update();
		RENDERER.Update(dt);
		RENDERER.RenderSimpleText("This text render from GAME! \nThe x and y values in the function isn't pixel \ncoordinates, it's char position. Every char is \n8x16 pixels in size. Use \\n to change line.\n\n  !Not all chars is supported!\n\nRight now it clear the whole output image as well (Tell me when to remove this).", 10, 2);

		RENDERER.Render();
		INPUT->Update();

		//unsigned int ids[50];
		//for (unsigned int i = 0; i < 50; ++i)
		//{
		//	ids[i] = world->CreateNewEntity();
		//	world->CreateComponentAndAddTo("Position", ids[i]);
		//	world->CreateComponentAndAddTo("Velocity", ids[i]);
		//}

		world->Update(dt);

		//for (unsigned int i = 0; i < 50; ++i)
		//{
		//	world->KillEntity(ids[i]);
		//}

		SDL_Event e;
		while (SDL_PollEvent(&e))
		{
			switch (e.type)
			{
			case SDL_WINDOWEVENT:
				RENDERER.PollEvent(e);
				break;
			case SDL_KEYDOWN:
			case SDL_KEYUP:
			case SDL_FINGERMOTION:
			case SDL_FINGERDOWN:
			case SDL_FINGERUP:
			case SDL_JOYAXISMOTION:
			case SDL_JOYBALLMOTION:
			case SDL_JOYHATMOTION:
			case SDL_JOYBUTTONDOWN:
			case SDL_JOYBUTTONUP:
			case SDL_MOUSEMOTION:
			case SDL_MOUSEBUTTONDOWN:
			case SDL_MOUSEBUTTONUP:
			case SDL_MOUSEWHEEL:
			case SDL_MULTIGESTURE:
				INPUT->PollEvent(e);
				break;
			}
		}

		// MOVE CUBE
		if (INPUT->GetKeyboard()->GetKeyState(SDL_SCANCODE_UP) == Input::InputState::DOWN)
			mat[100] *= glm::translate(vec3(0, 0, -0.01f));
		if (INPUT->GetKeyboard()->GetKeyState(SDL_SCANCODE_DOWN) == Input::InputState::DOWN)
			mat[100] *= glm::translate(vec3(0, 0, 0.01f));
		if (INPUT->GetKeyboard()->GetKeyState(SDL_SCANCODE_LEFT) == Input::InputState::DOWN)
			mat[100] *= glm::translate(vec3(-0.01f, 0, 0));
		if (INPUT->GetKeyboard()->GetKeyState(SDL_SCANCODE_RIGHT) == Input::InputState::DOWN)
			mat[100] *= glm::translate(vec3(0.01f, 0, 0));
		if (INPUT->GetKeyboard()->GetKeyState(SDL_SCANCODE_SPACE) == Input::InputState::DOWN)
			mat[100] *= glm::translate(vec3(0, 0.01f, 0));
		if (INPUT->GetKeyboard()->GetKeyState(SDL_SCANCODE_LSHIFT) == Input::InputState::DOWN)
			mat[100] *= glm::translate(vec3(0, -0.01f, 0));

		// MOVE CAMERA
		if (INPUT->GetKeyboard()->GetKeyState(SDL_SCANCODE_W) == Input::InputState::DOWN)
			RENDERER.GetCamera()->MoveForward(dt);
		if (INPUT->GetKeyboard()->GetKeyState(SDL_SCANCODE_S) == Input::InputState::DOWN)
			RENDERER.GetCamera()->MoveBackward(dt);
		if (INPUT->GetKeyboard()->GetKeyState(SDL_SCANCODE_A) == Input::InputState::DOWN)
			RENDERER.GetCamera()->MoveLeft(dt);
		if (INPUT->GetKeyboard()->GetKeyState(SDL_SCANCODE_D) == Input::InputState::DOWN)
			RENDERER.GetCamera()->MoveRight(dt);

		// ROTATE CAMERA
		if (INPUT->GetMouse()->GetButtonState(Input::LeftButton) == Input::InputState::DOWN)
		{
			int sizeX, sizeY;
			RENDERER.GetWindowSize(sizeX, sizeY);

			RENDERER.GetCamera()->UpdateMouse(sizeX*0.5, sizeY*0.5, INPUT->GetMouse()->GetX(), INPUT->GetMouse()->GetY());
			INPUT->GetMouse()->SetPosition(sizeX*0.5, sizeY*0.5);
			INPUT->GetMouse()->HideCursor(true);
		}
		else
			INPUT->GetMouse()->HideCursor(false);
		//-----------------------------------------------------------------------------------------------

		if (INPUT->GetKeyboard()->GetKeyState(SDL_SCANCODE_ESCAPE) == Input::InputState::PRESSED)
		{
			lol = false;
		}
	}

	delete(INPUT);
}

int main(int argc, char** argv)
{
	lol();
	Start();
	#ifdef WIN32
	_CrtDumpMemoryLeaks();
	#endif
	return 0;
}