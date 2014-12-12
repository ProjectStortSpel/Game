#include "GameCreator.h"
#include "Systems/MovementSystem.h"
#include "Systems/RenderSystem.h"
#include "Systems/CameraSystem.h"
#include "Systems/RotationSystem.h"
#include "Systems/ModelSystem.h"
#include "Systems/SyncEntitiesSystem.h"
#include "Systems/RenderRemoveSystem.h"
#include "Systems/ResetChangedSystem.h"
#include "Systems/PointlightSystem.h"

#include "NetworkInstance.h"
#include "ECSL/ECSL.h"
#include "ECSL/Managers/EntityTemplateManager.h"

#include "LuaBridge/ECSL/LuaSystem.h"

GameCreator::GameCreator() :
m_graphics(0), m_input(0), m_world(0), m_console(0), m_consoleManager(Console::ConsoleManager::GetInstance()), m_frameCounter(&Utility::FrameCounter::GetInstance())
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

	NetworkInstance::DestroyClient();
	NetworkInstance::DestroyServer();
	NetworkInstance::DestroyNetworkHelper();

	LuaEmbedder::Quit();

	delete(&ECSL::ComponentTypeManager::GetInstance());
	delete(&ECSL::EntityTemplateManager::GetInstance());
	delete(&Utility::FrameCounter::GetInstance());
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
	NetworkInstance::InitClient();
	NetworkInstance::InitServer();
	NetworkInstance::InitNetworkHelper(&m_world);

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

	if (!LuaEmbedder::Load("../../../Externals/content/scripting/storaspel/init.lua"))
	  return;
	
	auto componentTypes = ECSL::ComponentTypeManager::GetInstance().GetComponentTypes();
	for (auto it = componentTypes->begin(); it != componentTypes->end(); ++it)
	{
		worldCreator.AddComponentType(it->second->GetName());
		printf("%s added\n", it->second->GetName().c_str());
	}

	/*	This component has to be added last!	*/
	unsigned int numberOfComponents = ECSL::ComponentTypeManager::GetInstance().GetComponentTypeCount();
	unsigned int numberOfInts = ECSL::BitSet::GetIntCount(numberOfComponents);
	unsigned int numberOfBytes = numberOfInts*sizeof(ECSL::BitSet::DataType);
	std::map<std::string, ECSL::ComponentVariable> m_variables;
	ECSL::ComponentVariable start = ECSL::ComponentVariable("ChangedComponents", numberOfBytes);
	m_variables.insert(std::pair<std::string, ECSL::ComponentVariable>("ChangedComponents", start));
	std::map<unsigned int, ECSL::ComponentDataType> m_offsetToType;
	m_offsetToType[0] = ECSL::ComponentDataType::INT64;

	ECSL::ComponentType* changedComponents = new ECSL::ComponentType("ChangedComponents", ECSL::TableType::Array, m_variables, m_offsetToType, false);
	ECSL::ComponentTypeManager::GetInstance().AddComponentType(*changedComponents);
	worldCreator.AddComponentType("ChangedComponents");

	ECSL::ComponentType* changedComponentsNetwork = new ECSL::ComponentType("ChangedComponentsNetwork", ECSL::TableType::Array, m_variables, m_offsetToType, false);
	ECSL::ComponentTypeManager::GetInstance().AddComponentType(*changedComponentsNetwork);
	worldCreator.AddComponentType("ChangedComponentsNetwork");

	//NetworkMessagesSystem* nms = new NetworkMessagesSystem();
	//nms->SetConsole(&m_consoleManager);

	worldCreator.AddLuaSystemToCurrentGroup(new PointlightSystem(m_graphics));
	worldCreator.AddLuaSystemToCurrentGroup(new RotationSystem());
	worldCreator.AddLuaSystemToCurrentGroup(new CameraSystem(m_graphics));
	worldCreator.AddLuaSystemToCurrentGroup(new ModelSystem(m_graphics));
	worldCreator.AddLuaSystemToCurrentGroup(new RenderSystem(m_graphics));
	worldCreator.AddLuaSystemToCurrentGroup(new SyncEntitiesSystem());
	worldCreator.AddLuaSystemToCurrentGroup(new RenderRemoveSystem(m_graphics));
	worldCreator.AddLuaSystemToCurrentGroup(new ResetChangedSystem());

	m_world = worldCreator.CreateWorld(10000);
	LuaEmbedder::AddObject<ECSL::World>("World", m_world, "world");
	
	LuaEmbedder::CallMethods<LuaBridge::LuaSystem>("System", "PostInitialize");

}

void GameCreator::StartGame()
{
	/*	If atleast one object is not initialized the game can't start	*/
	if (!m_graphics || !m_input || !m_world || m_console)
		return;

	m_console = new GameConsole(m_graphics, m_world);

	m_consoleInput.SetTextHook(std::bind(&Console::ConsoleManager::ExecuteCommand, &m_consoleManager, std::placeholders::_1));
	m_consoleInput.SetActive(false);
	m_input->GetKeyboard()->StopTextInput();

	/*	Hook console	*/
	m_console->SetupHooks(&m_consoleManager);
	m_consoleManager.AddCommand("Reload", std::bind(&GameCreator::Reload, this, std::placeholders::_1));

	/*	Tempkod för ljus (LUA FIX)	*/
	unsigned int newLight = m_world->CreateNewEntity();
	unsigned int firstId = newLight;
	m_world->CreateComponentAndAddTo("Pointlight", newLight);

	newLight = m_world->CreateNewEntity();
	m_world->CreateComponentAndAddTo("Pointlight", newLight);

	newLight = m_world->CreateNewEntity();
	m_world->CreateComponentAndAddTo("Pointlight", newLight);


	float* pointlightData = (float*)m_world->GetComponent(firstId, "Pointlight", 0);
	for (int i = 0; i < 3; i++)
	{
		pointlightData[10 * i + 0] = i * 4 - 5.0;	//pos x
		pointlightData[10 * i + 1] = 2.0;		//pos y
		pointlightData[10 * i + 2] = 0.0;		//pos z
		pointlightData[10 * i + 3] = 0.6;		 //int x
		pointlightData[10 * i + 4] = 0.9;		 //int y
		pointlightData[10 * i + 5] = 0.5;		 //int z
		pointlightData[10 * i + 6] = 0.9;		//col x
		pointlightData[10 * i + 7] = 0.5;		//col y
		pointlightData[10 * i + 8] = 0.5;		//col z
		pointlightData[10 * i + 9] = 5.0;		 //range
	}

	while (true)
	{
		float dt = m_frameCounter->GetDeltaTime();

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

		m_frameCounter->Tick();
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
		printf("%d average fps\n", m_frameCounter->GetAverageFPS());
	}

	// History, arrows up/down
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


	if (m_input->GetKeyboard()->GetKeyState(SDL_SCANCODE_PAGEUP) == Input::InputState::PRESSED)
		m_consoleManager.ScrollUp();
	else if (m_input->GetKeyboard()->GetKeyState(SDL_SCANCODE_PAGEDOWN) == Input::InputState::PRESSED)
		m_consoleManager.ScrollDown();


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
	m_graphics->RenderSimpleText("Console:", 0, 30);
	m_graphics->RenderSimpleText(command, 9, 30);
	m_graphics->RenderSimpleText("_", 9 + command.length(), 30);

	auto history = m_consoleManager.GetHistory();
	for (int i = 0; i < history.size(); ++i)
		m_graphics->RenderSimpleText(history[i], 0, 30 - history.size() + i);

	auto match = m_consoleManager.GetFunctionMatch(command.c_str());
	m_graphics->RenderSimpleText(match, 9, 31);
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

void GameCreator::Reload(std::vector<Console::Argument>* _args)
{
  if (m_world)
	  delete m_world;
  NetworkInstance::DestroyClient();
  NetworkInstance::DestroyServer();
  NetworkInstance::DestroyNetworkHelper();
  LuaEmbedder::Quit();
  ECSL::ComponentTypeManager::GetInstance().Clear();
  ECSL::EntityTemplateManager::GetInstance().Clear();
  
  
  InitializeLua();
  m_graphics->Clear();
  InitializeNetwork();
  InitializeWorld();
}

