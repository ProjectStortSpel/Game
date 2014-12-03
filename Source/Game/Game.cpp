#include <SDL/SDL.h>
#include <iostream>
#include <string>
#include <algorithm>
#include "Network/ServerNetwork.h"
#include "Network/ClientNetwork.h"
#include "Network/PacketHandler.h"
#include "Timer.h"
#include "ECSL/ECSL.h"
#include "Input/InputWrapper.h"
#include "Renderer/GraphicDevice.h"

#ifdef WIN32
	#define _CRTDBG_MAP_ALLOC

	#ifdef _DEBUG
		#ifndef DBG_NEW
			#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
			#define new DBG_NEW
		#endif
	#endif  // _DEBUG


	#include <stdlib.h>
	#include <crtdbg.h>
#endif

int CardsInHand = 8;
int CardsToPlay = 5;
int waitforplayers;
mat4 mat[1000]; // SKA VARA I ENTITY/COMPONENT


std::string myName = "";

void ClearConsole()
{
#ifdef WIN32
	system("cls");
#else
	system("clear");
#endif
}

void LoadAlotOfBoxes(Renderer::GraphicDevice* r)
{
	// ADDING TEMP OBJECTS
	for (int x = 0; x < 10; x++)
	{
		for (int y = 0; y < 10; y++)
		{
			mat[y+x*10] = glm::translate(vec3(x - 5, -1, y - 5));
			r->LoadModel("content/models/cube/", "cube.object", &mat[y + x * 10]);
		}
	}
}

void TestECSL()
{
	//ECSL::ComponentTypeManager::GetInstance().LoadComponentTypesFromDirectory("content/components");
	//ECSL::WorldCreator worldCreator = ECSL::WorldCreator();
	//worldCreator.AddSystemGroup();
	//worldCreator.AddComponentType("Position");
	//worldCreator.AddComponentType("Velocity");

	//ECSL::World* world = worldCreator.CreateWorld(100);

	//int id = world->CreateNewEntity();
	//world->CreateComponentAndAddTo("Velocity", id);
	//world->CreateComponentAndAddTo("Position", id);

	//world->KillEntity(id);

	//delete world;
	//delete(&ECSL::ComponentTypeManager::GetInstance());
	//delete(&ECSL::BitSet::BitSetConverter::GetInstance());
}


int main(int argc, char** argv)
{
#ifdef WIN32
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
	
	Timer timer;

	Renderer::GraphicDevice RENDERER = Renderer::GraphicDevice();
	Input::InputWrapper* INPUT = &Input::InputWrapper::GetInstance();
	RENDERER.Init();

	ServerNetwork server;
	server.Start(6112, "", 8);

	TestECSL();
	LoadAlotOfBoxes(&RENDERER);
	mat[100] = glm::translate(vec3(0, 0, 0));
	int modelid = RENDERER.LoadModel("content/models/cube/", "cube.object", &mat[100]); // LOADMODEL RETURNS THE MODELID
	RENDERER.ChangeModelTexture(modelid, "content/models/cube/NM_tst.png"); // CHANGING TEXTURE ON MODELID

	bool lol = true;
	float cd = 1.0f;
	while (lol)
	{
		// DT COUNTER
		float dt = timer.ElapsedTimeInSeconds();
		timer.Reset();

		INPUT->Update();
		RENDERER.Update(dt);
		//RENDERER.RenderSimpleText("This text render from GAME! \nThe x and y values in the function isn't pixel \ncoordinates, it's char position. Every char is \n8x16 pixels in size. Use \\n to change line.\n\n  !Not all chars is supported!\n\nRight now it clear the whole output image as well (Tell me when to remove this).", 10, 2);
		

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
			mat[50] *= glm::translate(vec3(0, 0, -0.01f)); 
		if (INPUT->GetKeyboard()->GetKeyState(SDL_SCANCODE_DOWN) == Input::InputState::DOWN)
			mat[50] *= glm::translate(vec3(0, 0, 0.01f));
		if (INPUT->GetKeyboard()->GetKeyState(SDL_SCANCODE_LEFT) == Input::InputState::DOWN)
			mat[50] *= glm::translate(vec3(-0.01f, 0, 0));
		if (INPUT->GetKeyboard()->GetKeyState(SDL_SCANCODE_RIGHT) == Input::InputState::DOWN)
			mat[50] *= glm::translate(vec3(0.01f, 0, 0)); 
		if (INPUT->GetKeyboard()->GetKeyState(SDL_SCANCODE_SPACE) == Input::InputState::DOWN)
			mat[50] *= glm::translate(vec3(0, 0.01f, 0));
		if (INPUT->GetKeyboard()->GetKeyState(SDL_SCANCODE_LSHIFT) == Input::InputState::DOWN)
			mat[50] *= glm::translate(vec3(0, -0.01f, 0));

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
	return 0;
}