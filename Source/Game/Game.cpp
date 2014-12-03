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

	}
	void Initialize()
	{
		AddComponentTypeToFilter("MegaAwesomeComponent", ECSL::FilterType::Mandatory);
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

Renderer::GraphicDevice*	Graphics;
Input::InputWrapper*		InputWrapper;


void PollSDLEvents()
{
	SDL_Event e;
	while (SDL_PollEvent(&e))
	{
		switch (e.type)
		{
		case SDL_WINDOWEVENT:
			Graphics->PollEvent(e);
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
			InputWrapper->PollEvent(e);
			break;
		}
	}
}
void Start()
{
	Graphics = new Renderer::GraphicDevice();
	InputWrapper = &Input::InputWrapper::GetInstance();

	Graphics->Init();
	LoadAlotOfBoxes(Graphics);

	mat[100] = glm::translate(vec3(0, 0, 0));

	int modelid = Graphics->LoadModel("content/models/cube/", "cube.object", &mat[100]); // LOADMODEL RETURNS THE MODELID
	Graphics->ChangeModelTexture(modelid, "content/models/cube/NM_tst.png"); // CHANGING TEXTURE ON MODELID


	/*	How to create a world	*/
	ComponentTypeManager::GetInstance().LoadComponentTypesFromDirectory("content/components");	//	Load all component types from 'content/components'

	/*	1.	Create the 'World Creator'	*/
	ECSL::WorldCreator worldCreator = ECSL::WorldCreator();
	/*	2.	Add systems*/
	worldCreator.AddSystemGroup();
	worldCreator.AddSystemToCurrentGroup<TestSystem2>();
	worldCreator.AddLuaSystemToCurrentGroup(new TestSystem());

	/*	3.	Add all loaded component types	*/
	auto componentTypes = ComponentTypeManager::GetInstance().GetComponentTypes();
	for (auto it = componentTypes->begin(); it != componentTypes->end(); ++it)
		worldCreator.AddComponentType(it->second->GetName());

	/*	4.	Create the world	*/
	ECSL::World* world = worldCreator.CreateWorld(50000);
	int cId = ComponentTypeManager::GetInstance().GetTableId("MegaAwesomeComponent");
	for (int n = 0; n < 50000; ++n)
	{
		int id = world->CreateNewEntity();
		world->CreateComponentAndAddTo(cId, id);
	}
			


	/*	Temporary game loop	*/
	Timer gameTimer;
	while (true)
	{
		float dt = gameTimer.ElapsedTimeInSeconds();
		gameTimer.Reset();

		InputWrapper->Update();
		Graphics->Update(dt);
		PollSDLEvents();


		world->Update(dt);
		Graphics->RenderSimpleText("This text render from GAME! \nThe x and y values in the function isn't pixel \ncoordinates, it's char position. Every char is \n8x16 pixels in size. Use \\n to change line.\n\n  !Not all chars is supported!\n\nRight now it clear the whole output image as well (Tell me when to remove this).", 10, 2);


		// MOVE CUBE
		if (InputWrapper->GetKeyboard()->GetKeyState(SDL_SCANCODE_UP) == Input::InputState::DOWN)
			mat[100] *= glm::translate(vec3(0, 0, -0.01f));
		if (InputWrapper->GetKeyboard()->GetKeyState(SDL_SCANCODE_DOWN) == Input::InputState::DOWN)
			mat[100] *= glm::translate(vec3(0, 0, 0.01f));
		if (InputWrapper->GetKeyboard()->GetKeyState(SDL_SCANCODE_LEFT) == Input::InputState::DOWN)
			mat[100] *= glm::translate(vec3(-0.01f, 0, 0));
		if (InputWrapper->GetKeyboard()->GetKeyState(SDL_SCANCODE_RIGHT) == Input::InputState::DOWN)
			mat[100] *= glm::translate(vec3(0.01f, 0, 0));
		if (InputWrapper->GetKeyboard()->GetKeyState(SDL_SCANCODE_SPACE) == Input::InputState::DOWN)
			mat[100] *= glm::translate(vec3(0, 0.01f, 0));
		if (InputWrapper->GetKeyboard()->GetKeyState(SDL_SCANCODE_LSHIFT) == Input::InputState::DOWN)
			mat[100] *= glm::translate(vec3(0, -0.01f, 0));

		// MOVE CAMERA
		if (InputWrapper->GetKeyboard()->GetKeyState(SDL_SCANCODE_W) == Input::InputState::DOWN)
			Graphics->GetCamera()->MoveForward(dt);
		if (InputWrapper->GetKeyboard()->GetKeyState(SDL_SCANCODE_S) == Input::InputState::DOWN)
			Graphics->GetCamera()->MoveBackward(dt);
		if (InputWrapper->GetKeyboard()->GetKeyState(SDL_SCANCODE_A) == Input::InputState::DOWN)
			Graphics->GetCamera()->MoveLeft(dt);
		if (InputWrapper->GetKeyboard()->GetKeyState(SDL_SCANCODE_D) == Input::InputState::DOWN)
			Graphics->GetCamera()->MoveRight(dt);

		// ROTATE CAMERA
		if (InputWrapper->GetMouse()->GetButtonState(Input::LeftButton) == Input::InputState::DOWN)
		{
			int sizeX, sizeY;
			Graphics->GetWindowSize(sizeX, sizeY);

			Graphics->GetCamera()->UpdateMouse(sizeX*0.5, sizeY*0.5, InputWrapper->GetMouse()->GetX(), InputWrapper->GetMouse()->GetY());
			InputWrapper->GetMouse()->SetPosition(sizeX*0.5, sizeY*0.5);
			InputWrapper->GetMouse()->HideCursor(true);
		}
		else
			InputWrapper->GetMouse()->HideCursor(false);

		if (InputWrapper->GetKeyboard()->GetKeyState(SDL_SCANCODE_ESCAPE) == Input::InputState::PRESSED)
			break;




		Graphics->Render();
	}

	delete(world);
	delete(&ComponentTypeManager::GetInstance());
	delete(InputWrapper);
	delete(Graphics);
}

int main(int argc, char** argv)
{
	Start();
	#ifdef WIN32
	_CrtDumpMemoryLeaks();
	#endif
	return 0;
}