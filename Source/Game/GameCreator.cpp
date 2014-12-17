#include <algorithm>
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
	NetworkInstance::DestroyClient();
	NetworkInstance::DestroyServer();
	NetworkInstance::DestroyNetworkHelper();

	if (m_world)
		delete m_world;

	if (m_graphics)
		delete m_graphics;

	if (m_input)
		delete m_input;

	if (m_console)
		delete m_console;	

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

	Network::NetMessageHook hook = std::bind(&GameCreator::LuaPacket, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
	NetworkInstance::GetClient()->AddNetworkHook("LuaPacket", hook);
	NetworkInstance::GetServer()->AddNetworkHook("LuaPacket", hook);

	Network::NetEvent netEvent = std::bind(&GameCreator::OnConnectedToServer, this, std::placeholders::_1, std::placeholders::_2);
	NetworkInstance::GetClient()->SetOnConnectedToServer(netEvent);
}

void GameCreator::InitializeLua() 
{
	LuaEmbedder::Init();
	LuaBridge::Embed();
}

void GameCreator::InitializeWorld(std::string _gameMode)
{
	ECSL::WorldCreator worldCreator = ECSL::WorldCreator();
	LuaEmbedder::AddObject<ECSL::WorldCreator>("WorldCreator", &worldCreator, "worldCreator");

	std::stringstream gameMode;
	gameMode << "../../../Externals/content/scripting/";
	gameMode << _gameMode;
	gameMode << "/init.lua";
	std::string lol = gameMode.str();
	if (!LuaEmbedder::Load(gameMode.str()))
	  return;

	m_gameMode = _gameMode;
	
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

	worldCreator.AddLuaSystemToCurrentGroup(new SyncEntitiesSystem());
	//worldCreator.AddLuaSystemToCurrentGroup(new ReceivePacketSystem());
	worldCreator.AddLuaSystemToCurrentGroup(new RenderSystem(m_graphics));
	//worldCreator.AddLuaSystemToCurrentGroup(new ReconnectSystem());
	worldCreator.AddLuaSystemToCurrentGroup(new RenderRemoveSystem(m_graphics));

	worldCreator.AddLuaSystemToCurrentGroup(new ResetChangedSystem());

	m_world = worldCreator.CreateWorld(1000);
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
	m_consoleManager.AddCommand("GameMode", std::bind(&GameCreator::GameMode, this, std::placeholders::_1));
	
	float maxDeltaTime = (float)(1.0f / 20.0f);
	while (true)
	{
		float dt = std::min(maxDeltaTime, m_frameCounter->GetDeltaTime());

		/*	Collect all input	*/
		m_input->Update();
		PollSDLEvent();
		m_consoleInput.Update();

		/*	Update graphics	*/
		m_graphics->Update(dt);
		
		/*	Update world (systems, entities etc)	*/
		m_world->Update(dt);
		
		UpdateNetwork(dt);
		//LuaEmbedder::CollectGarbage(1);

		std::stringstream ss;
		ss << "Lua memory usage: " << LuaEmbedder::GetMemoryUsage() << " bytes";
		m_graphics->RenderSimpleText(ss.str(), 20, 1);

		UpdateConsole();

		if (m_input->GetKeyboard()->GetKeyState(SDL_SCANCODE_ESCAPE) == Input::InputState::PRESSED)
			break;

		RenderConsole();
		m_graphics->Render();

		m_frameCounter->Tick();
	}
}

void GameCreator::UpdateNetwork(float _dt)
{
	Network::ServerNetwork* server = NetworkInstance::GetServer();
	if (server->IsRunning())
	{
		server->Update(_dt);
		while (server->PopAndExecutePacket() > 0) {}
	}

	Network::ClientNetwork* client = NetworkInstance::GetClient();
	if (client->IsConnected())
	{
		client->Update(_dt);
		while (client->PopAndExecutePacket() > 0) {}
	}
}

void GameCreator::LuaPacket(Network::PacketHandler* _ph, uint64_t& _id, Network::NetConnection& _nc)
{
	std::ostringstream ss;
	ss << _id;

	char* function = _ph->ReadString(_id);

	LuaEmbedder::PushString(ss.str());
	LuaEmbedder::PushString(_nc.GetIpAddress());
	LuaEmbedder::PushInt((int)_nc.GetPort());
	LuaEmbedder::CallSavedFunction(function, 3);
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

	Network::ClientNetwork* client = NetworkInstance::GetClient();

	if (client->IsConnected())
	{
		std::ostringstream ss;
		ss << "Ping: " << client->GetPing() << "ms";
		std::string s(ss.str());

		m_graphics->RenderSimpleText(s, 0, 32);
	}
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
	NetworkInstance::GetNetworkHelper()->ResetNetworkMaps();
	bool server = LuaEmbedder::PullBool("Server");
	bool client = LuaEmbedder::PullBool("Client");
	LuaEmbedder::Quit();
	ECSL::ComponentTypeManager::GetInstance().Clear();
	ECSL::EntityTemplateManager::GetInstance().Clear();


	InitializeLua();
	LuaEmbedder::AddBool("Server", server);
	LuaEmbedder::AddBool("Client", client);
	m_graphics->Clear();
	InitializeWorld(m_gameMode);
	m_console->SetWorld(m_world);
}

void GameCreator::OnConnectedToServer(Network::NetConnection _nc, const char* _message)
{
 	std::vector<Console::Argument>* args = new std::vector<Console::Argument>();
	m_gameMode = "storaspel";
	Reload(args);
	delete args;
}

void GameCreator::GameMode(std::vector<Console::Argument>* _args)
{
	if (_args->size() == 0)
	{
		m_gameMode = "storaspel";
		Reload(_args);
	}
	else if (_args->size() == 1)
	{
		if (_args->at(0).ArgType == Console::ArgumentType::Text)
		{
			std::string gameMode = _args->at(0).Text;
			m_gameMode = gameMode;

			Reload(_args);
		}
	}
	return;	
}
