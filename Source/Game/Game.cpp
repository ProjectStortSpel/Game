#include <SDL/SDL.h>
#include <ECSL/ECSL.h>
#include "Input/InputWrapper.h"
#include "Renderer/GraphicDevice.h"
#include "Timer.h"

#ifdef WIN32
	#define _CRTDBG_MAP_ALLOC
	#include <stdlib.h>
	#include <crtdbg.h>
#endif

using namespace ECSL;

mat4 mat[1000];

class TestSystem : public ECSL::System
{
public:
	TestSystem() { }
	~TestSystem() { }

	void Update(float _dt)
	{
		printf("Testsystem run()\n");
	}
	void Initialize()
	{
		AddComponentTypeToFilter("Velocity", ECSL::FilterType::Mandatory);
		AddComponentTypeToFilter("Position", ECSL::FilterType::Excluded);
		//AddComponentTypeToFilter("Velocity", ECSL::FilterType::RequiresOneOf);
		//AddComponentTypeToFilter("Velocity", ECSL::ComponentFilter::RequiresOneOf);
		//AddComponentTypeToFilter("Position", ECSL::ComponentFilter::Excluded);

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
class TestSystem2 : public ECSL::System
{
public:
	TestSystem2() { }
	~TestSystem2() { }

	void Update(float _dt)
	{
		printf("Testsystem2 run()\n");
	}
	void Initialize()
	{
		AddComponentTypeToFilter("Velocity", ECSL::FilterType::Mandatory);
		//AddComponentTypeToFilter("Velocity", ECSL::FilterType::RequiresOneOf);
		//AddComponentTypeToFilter("Velocity", ECSL::ComponentFilter::RequiresOneOf);
		//AddComponentTypeToFilter("Position", ECSL::ComponentFilter::Excluded);

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

void lol()
{
	ComponentTypeManager::GetInstance().LoadComponentTypesFromDirectory("content/components");
	ECSL::WorldCreator worldCreator = ECSL::WorldCreator();
	worldCreator.AddSystemGroup();
	worldCreator.AddSystemToCurrentGroup<TestSystem2>();
	worldCreator.AddLuaSystemToCurrentGroup(new TestSystem());
	auto componentTypes = ComponentTypeManager::GetInstance().GetComponentTypes();
	for (auto it = componentTypes->begin(); it != componentTypes->end(); ++it)
		worldCreator.AddComponentType(it->second->GetName());
	ECSL::World* world = worldCreator.CreateWorld(100);

	int id = world->CreateNewEntity();
	world->CreateComponentAndAddTo("Velocity", id);
	//world->CreateComponentAndAddTo("Position", id);
	//world->RemoveComponentFrom("Position", id);
	//world->CreateComponentAndAddTo("Position", id);

	//world->KillEntity(id);

	world->Update(0.01f);

	//world->KillEntity(id);
	world->CreateComponentAndAddTo("Position", id);

	world->Update(0.01f);

	world->RemoveComponentFrom("Position", id);

	world->Update(0.01f);

	delete(world);
	delete(&ComponentTypeManager::GetInstance());
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
	Start();
	#ifdef WIN32
	_CrtDumpMemoryLeaks();
	#endif
	return 0;
}