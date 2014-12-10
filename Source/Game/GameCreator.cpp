#include "GameCreator.h"
#include "Timer.h"
#include "Systems/MovementSystem.h"
#include "Systems/RenderSystem.h"
#include "Systems/CameraSystem.h"
#include "Systems/RotationSystem.h"
#include "Systems/ModelSystem.h"
#include "Systems/ReceivePacketSystem.h"

#include "ECSL/ECSL.h"
#include "ECSL/Managers/EntityTemplateManager.h"

GameCreator::GameCreator() :
m_graphics(0), m_input(0), m_world(0), m_console(0), m_client(0), m_server(0)
{

}

GameCreator::~GameCreator()
{
	if (m_world)
		delete m_world;

	if (m_graphics)
		delete m_graphics;

	if (m_input)
		delete m_input;

	if (m_console)
		delete m_console;

	if (m_client)
		delete m_client;

	if (m_server)
		delete m_server;

	LuaEmbedder::Quit();

	delete(&ECSL::ComponentTypeManager::GetInstance());
}

glm::mat4 mat[1000];

void SpawnStuff(Renderer::GraphicDevice* graphics)
{
	for (int x = 0; x < 10; x++)
	{
		for (int y = 0; y < 10; y++)
		{
			mat[y + x * 10] = glm::translate(vec3(x - 5, -1, y - 5));
			graphics->LoadModel("content/models/default_tile/", "default.object", &mat[y + x * 10]);
		}
	}
}

void GameCreator::InitializeGraphics()
{
	m_graphics = new Renderer::GraphicDevice();
	m_graphics->Init();
	LuaEmbedder::AddObject<Renderer::GraphicDevice>("GraphicDevice", m_graphics, "graphics");
}

void GameCreator::InitializeConsole()
{
	//m_consoleManager->AddCommand("SpawnWorld", &SpawnShit);
}

void GameCreator::InitializeInput()
{
	m_input = &Input::InputWrapper::GetInstance();
}

void GameCreator::InitializeNetwork()
{
	m_client = new Network::ClientNetwork();
	m_server = new Network::ServerNetwork();
}

void GameCreator::InitializeLua()
{
	LuaEmbedder::Init();
	LuaBridge::Embed();
}

void GameCreator::InitializeWorld()
{
	//ECSL::ComponentTypeManager::GetInstance().LoadComponentTypesFromDirectory("content/components");
	//ECSL::EntityTemplateManager::GetInstance().LoadComponentTypesFromDirectory("content/scripting/storaspel/templates");

	ECSL::WorldCreator worldCreator = ECSL::WorldCreator();
	LuaEmbedder::AddObject<ECSL::WorldCreator>("WorldCreator", &worldCreator, "worldCreator");

	LuaEmbedder::Load("../../../Externals/content/scripting/storaspel/init.lua");

	auto componentTypes = ECSL::ComponentTypeManager::GetInstance().GetComponentTypes();
	for (auto it = componentTypes->begin(); it != componentTypes->end(); ++it)
	{
		worldCreator.AddComponentType(it->second->GetName());
		printf("%s added\n", it->second->GetName().c_str());
	}

	//worldCreator.AddSystemGroup();
	//worldCreator.AddSystemToCurrentGroup<MovementSystem>();
	worldCreator.AddLuaSystemToCurrentGroup(new RotationSystem());
	worldCreator.AddLuaSystemToCurrentGroup(new CameraSystem(m_graphics));
	worldCreator.AddLuaSystemToCurrentGroup(new ModelSystem(m_graphics));
	worldCreator.AddLuaSystemToCurrentGroup(new RenderSystem(m_graphics));
	worldCreator.AddLuaSystemToCurrentGroup(new ReceivePacketSystem(m_client, m_server));

	
	m_world = worldCreator.CreateWorld(10000);
	LuaEmbedder::AddObject<ECSL::World>("World", m_world, "world");
	
	LuaEmbedder::CallMethods<ECSL::System>("System", "PostInitialize");
}

void GameCreator::StartGame()
{
	/*	If atleast one object is not initialized the game can't start	*/
	if (!m_graphics || !m_input || !m_world || m_console)
		return;

	m_console = new GameConsole(m_graphics, m_world, m_client, m_server);

	m_consoleInput.SetTextHook(std::bind(&Console::ConsoleManager::ExecuteCommand, &m_consoleManager, std::placeholders::_1));
	m_consoleInput.SetActive(true);

	/*	Hook console	*/
	m_console->SetupHooks(&m_consoleManager);
	
	/*	FULKOD START	*/
	for (int x = 0; x < 10; x++)
	{
		for (int y = 0; y < 10; y++)
		{
			std::string command;
			if ((x + y) % 2)
			{
				command += "createobject hole_test Hole/ ";
			}
			else
			{
				command += "createobject grass Default_Tile/ ";
			}
			command += std::to_string(x);
			command.append(" ");
			command += std::to_string(-1);
			command.append(" ");
			command += std::to_string(y);
			command.append("");
			m_consoleManager.ExecuteCommand(command.c_str());
		}
	}
	/*	FULKOD END		*/

	Timer gameTimer;
	while (true)
	{
		float dt = gameTimer.ElapsedTimeInSeconds();
		gameTimer.Reset(); 

		/*	Collect all input	*/
		m_input->Update();
		PollSDLEvent();
		m_consoleInput.Update();

		/*	Update graphics	*/
		m_graphics->Update(dt);
		
		/*	Update world (systems, entities etc)	*/
		m_world->Update(dt);

		UpdateConsole();

		if (m_input->GetKeyboard()->GetKeyState(SDL_SCANCODE_ESCAPE) == Input::InputState::PRESSED)
			break;

		RenderConsole();
		m_graphics->Render();
	}
}

void GameCreator::UpdateConsole()
{
	/*	Toggle console	*/
	if (m_input->GetKeyboard()->GetKeyState(SDL_SCANCODE_GRAVE) == Input::InputState::PRESSED)
	{
		if (m_input->GetKeyboard()->IsTextInputActive())
		{
			m_input->GetKeyboard()->StopTextInput();
			m_consoleInput.SetActive(false);
		}
		else
		{
			m_input->GetKeyboard()->StartTextInput();
			m_consoleInput.SetActive(true);
			m_input->GetKeyboard()->ResetTextInput();
		}
	}




	// MOVE ?!
	if (m_input->GetKeyboard()->GetKeyState(SDL_SCANCODE_UP) == Input::InputState::PRESSED)
	{

		auto previous = m_consoleManager.GetPreviousHistory();
		if(previous)
			m_input->GetKeyboard()->SetTextInput(previous);

	}
	else if (m_input->GetKeyboard()->GetKeyState(SDL_SCANCODE_DOWN) == Input::InputState::PRESSED)
	{
		auto next = m_consoleManager.GetNextHistory();
		if (next)
			m_input->GetKeyboard()->SetTextInput(next);
	}



	if (m_input->GetKeyboard()->GetKeyState(SDL_SCANCODE_TAB) == Input::InputState::PRESSED)
	{
		auto match = m_consoleManager.GetMatch();

		if (match != "")
			m_input->GetKeyboard()->SetTextInput(match);
	}
}

void GameCreator::RenderConsole()
{
	if (!m_input->GetKeyboard()->IsTextInputActive())
		return;

	std::string command = m_consoleInput.GetText();
	m_graphics->RenderSimpleText("Console:", 0, 10);
	m_graphics->RenderSimpleText(command, 9, 10);
	m_graphics->RenderSimpleText("_", 9 + command.length(), 10);

	auto history = m_consoleManager.GetHistory();
	for (int i = 0; i < history.size(); ++i)
		m_graphics->RenderSimpleText(history[i], 0, 10 - history.size() + i);

	auto match = m_consoleManager.GetFunctionMatch(command.c_str());
	m_graphics->RenderSimpleText(match, 9, 11);
}

void GameCreator::PollSDLEvent()
{
	SDL_Event e;
	while (SDL_PollEvent(&e))
	{
		switch (e.type)
		{
		case SDL_WINDOWEVENT:
			m_graphics->PollEvent(e);
			break;
		case SDL_KEYDOWN:
		case SDL_KEYUP:
		case SDL_FINGERMOTION:
		case SDL_FINGERDOWN:
		case SDL_FINGERUP:
		case SDL_TEXTINPUT:
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
			m_input->PollEvent(e);
			break;
		}
	}
}