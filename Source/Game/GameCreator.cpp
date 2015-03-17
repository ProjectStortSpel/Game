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
#include "Systems/DirectionalLightSystem.h"
#include "Systems/MasterServerSystem.h"
#include "Systems/SlerpRotationSystem.h"
#include "Systems/AddTextToTextureSystem.h"
#include "Systems/ParticleSystem.h"
#include "Systems/SimulateFrameSpikeSystem.h"
#include "Systems/GenerateIslandSystem.h"

#include "Network/NetworkInstance.h"
#include "ECSL/ECSL.h"
#include "ECSL/Managers/EntityTemplateManager.h"

#include "LuaBridge/ECSL/LuaSystem.h"
#include "LuaBridge/ECSL/LuaWorldCreator.h"
#include "LuaBridge/Renderer/LuaGraphicDevice.h"
#include "LuaBridge/ECSL/LuaEntityTemplateManager.h"
#include "LuaBridge/Network/LuaNetwork.h"
#include "LuaBridge/Resource/LuaResource.h"
#include "Game/Network/ConnectHelper.h"
#include "Game/Network/ClientManager.h"
#include "FileSystem/File.h"

#include "Game/ResourceManager.h"
#include "FileSystem/Directory.h"

#include "Logger/Managers/Logger.h"

#include "Pathfinder.h"

#include "Game/HomePath.h"

#include <iomanip>

#include "LoadingScreen.h"

GameCreator::GameCreator() :
m_graphics(0), m_input(0), m_clientWorld(0), m_serverWorld(0), m_clientWorldProfiler(0), m_serverWorldProfiler(0), m_console(0), m_remoteConsole(0), m_consoleManager(Console::ConsoleManager::GetInstance()), m_frameCounter(new Utility::FrameCounter()), m_running(true),
m_graphicalSystems(std::vector<GraphicalSystem*>()), m_timeScale(1.0f)
{
  
}

GameCreator::~GameCreator()
{
	NetworkInstance::DestroyClient();
    NetworkInstance::DestroyServer();
    NetworkInstance::DestroyClientNetworkHelper();
    NetworkInstance::DestroyServerNetworkHelper();

	Pathfinder::Destroy();

	if (m_clientWorld)
		delete m_clientWorld;
    
    if (m_serverWorld)
        delete m_serverWorld;
    
	Audio::Quit();

	if (m_graphics)
	{
		TextRenderer::Clean();
		delete m_graphics;
		SDL_Quit();
	}

	if (m_input)
		delete m_input;

	if (m_console)
		delete m_console;	

	if (m_remoteConsole)
		delete m_remoteConsole;

	if (m_clientWorldProfiler)
		delete m_clientWorldProfiler;

	if (m_serverWorldProfiler)
		delete m_serverWorldProfiler;

	LuaEmbedder::Quit();

	delete(&ECSL::ComponentTypeManager::GetInstance());
	delete(&ECSL::EntityTemplateManager::GetInstance());
	delete(&ECSL::DataLogger::GetInstance());
	delete(&MPL::TaskManager::GetInstance());
	delete(m_frameCounter);
}

void GameCreator::InitializeAudio()
{
	Audio::Init();
}

bool GameCreator::InitializeGraphics()
{
#if defined(__IOS__) || defined(__ANDROID__)
	m_graphics = new Renderer::GraphicsLow();
    m_graphics->Init();
#else
    m_graphics = new Renderer::GraphicsHigh();
    if (!m_graphics->Init())
    {
        SDL_Log("Switching to OpenGL 4.0");
        delete(m_graphics);
        m_graphics = new Renderer::GraphicsLow();
        if (!m_graphics->Init())
        {
            SDL_Log("Failed to initialize graphics.");
            return false;
        }
    }
#endif
	m_graphics->AddFont("content/fonts/barthowheel.ttf", 72);
	m_graphics->AddFont("content/fonts/verdanab.ttf", 72);
	LoadingScreen::GetInstance().SetGraphicsDevice(m_graphics);
    return true;
	//LuaEmbedder::AddObject<Renderer::GraphicDevice>("GraphicDevice", m_graphics, "graphics");
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
	NetworkInstance::InitClientNetworkHelper(&m_clientWorld);
    NetworkInstance::InitServerNetworkHelper(&m_serverWorld);

	//hook = std::bind(&GameCreator::NetworkGameModeFiles, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
	//NetworkInstance::GetClient()->AddNetworkHook("GamemodeFiles", hook);

	ConnectHelper::LoadGameModeHook GMhook = std::bind(&GameCreator::GameMode, this, std::placeholders::_1);
	ConnectHelper::SetLoadGameModeHook(GMhook);

	InitializeNetworkEvents(false);

}

void GameCreator::InitializeNetworkEvents(bool _allowEntities)
{
	NetworkInstance::GetClient()->ResetNetworkEvents();
	NetworkInstance::GetServer()->ResetNetworkEvents();

	LuaBridge::LuaNetworkEvents::Clear();

	//Client NetEvents
	Network::NetEvent netEvent;
	netEvent = std::bind(&GameCreator::OnBannedFromServer, this, std::placeholders::_1, std::placeholders::_2);
	NetworkInstance::GetClient()->SetOnBannedFromServer(netEvent);

	netEvent = std::bind(&GameCreator::OnConnectedToServer, this, std::placeholders::_1, std::placeholders::_2);
	NetworkInstance::GetClient()->SetOnConnectedToServer(netEvent);

	netEvent = std::bind(&GameCreator::OnDisconnectedFromServer, this, std::placeholders::_1, std::placeholders::_2);
	NetworkInstance::GetClient()->SetOnDisconnectedFromServer(netEvent);

	netEvent = std::bind(&GameCreator::OnFailedToConnect, this, std::placeholders::_1, std::placeholders::_2);
	NetworkInstance::GetClient()->SetOnFailedToConnect(netEvent);

	netEvent = std::bind(&GameCreator::OnKickedFromServer, this, std::placeholders::_1, std::placeholders::_2);
	NetworkInstance::GetClient()->SetOnKickedFromServer(netEvent);

	netEvent = std::bind(&GameCreator::OnPasswordInvalid, this, std::placeholders::_1, std::placeholders::_2);
	NetworkInstance::GetClient()->SetOnPasswordInvalid(netEvent);

	netEvent = std::bind(&GameCreator::OnRemotePlayerBanned, this, std::placeholders::_1, std::placeholders::_2);
	NetworkInstance::GetClient()->SetOnRemotePlayerBanned(netEvent);

	netEvent = std::bind(&GameCreator::OnRemotePlayerConnected, this, std::placeholders::_1, std::placeholders::_2);
	NetworkInstance::GetClient()->SetOnRemotePlayerConnected(netEvent);

	netEvent = std::bind(&GameCreator::OnRemotePlayerDisconnected, this, std::placeholders::_1, std::placeholders::_2);
	NetworkInstance::GetClient()->SetOnRemotePlayerDisconnected(netEvent);

	netEvent = std::bind(&GameCreator::OnRemotePlayerKicked, this, std::placeholders::_1, std::placeholders::_2);
	NetworkInstance::GetClient()->SetOnRemotePlayerKicked(netEvent);

	netEvent = std::bind(&GameCreator::OnServerFull, this, std::placeholders::_1, std::placeholders::_2);
	NetworkInstance::GetClient()->SetOnServerFull(netEvent);

	netEvent = std::bind(&GameCreator::OnTimedOutFromServer, this, std::placeholders::_1, std::placeholders::_2);
	NetworkInstance::GetClient()->SetOnTimedOutFromServer(netEvent);

	//Server NetEvents
	netEvent = std::bind(&GameCreator::OnPlayerConnected, this, std::placeholders::_1, std::placeholders::_2);
	NetworkInstance::GetServer()->SetOnPlayerConnected(netEvent);

	netEvent = std::bind(&GameCreator::OnPlayerDisconnected, this, std::placeholders::_1, std::placeholders::_2);
	NetworkInstance::GetServer()->SetOnPlayerDisconnected(netEvent);

	netEvent = std::bind(&GameCreator::OnPlayerTimedOut, this, std::placeholders::_1, std::placeholders::_2);
	NetworkInstance::GetServer()->SetOnPlayerTimedOut(netEvent);


	Network::NetMessageHook hook = std::bind(&GameCreator::LuaPacket, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
	NetworkInstance::GetClient()->AddNetworkHook("LuaPacket", hook);
	NetworkInstance::GetServer()->AddNetworkHook("LuaPacket", hook);

	hook = std::bind(&GameCreator::NetworkGameMode, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
	NetworkInstance::GetClient()->AddNetworkHook("Gamemode", hook);

	ConnectHelper::Initialize();
	ClientManager::Initialize();

	SAFE_DELETE(m_remoteConsole);
	m_remoteConsole = new RemoteConsole();

	if (_allowEntities)
	{
		//Entity hooks
		Network::NetMessageHook hook = std::bind(&NetworkHelper::ReceiveEntityAll, NetworkInstance::GetClientNetworkHelper(), std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
		NetworkInstance::GetClient()->AddNetworkHook("Entity", hook);

		hook = std::bind(&NetworkHelper::ReceiveEntityDelta, NetworkInstance::GetClientNetworkHelper(), std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
		NetworkInstance::GetClient()->AddNetworkHook("EntityDelta", hook);

		hook = std::bind(&NetworkHelper::ReceiveEntityKill, NetworkInstance::GetClientNetworkHelper(), std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
		NetworkInstance::GetClient()->AddNetworkHook("EntityKill", hook);
	}
}

void GameCreator::InitializeThreads()
{
	MPL::TaskManager::GetInstance().CreateSlaves();
}

void GameCreator::InitializeLobby()
{
    GameMode("lobbythreaded");
    //InitializeWorld("lobbythreaded", WorldType::Client, true);
    //InitializeWorld("lobbythreaded", WorldType::Server, false);
}

void GameCreator::InitializeLua(WorldType _worldType)
{
    if (_worldType == WorldType::Client)
    {
        m_clientLuaState = LuaEmbedder::CreateState();
        LuaBridge::Embed(m_clientLuaState);
        
        LuaEmbedder::AddBool(m_clientLuaState, "Server", false);
        LuaEmbedder::AddBool(m_clientLuaState, "Client", true);
    }
    else
    {
        m_serverLuaState = LuaEmbedder::CreateState();
        LuaBridge::Embed(m_serverLuaState);
        
        LuaEmbedder::AddBool(m_serverLuaState, "Server", true);
        LuaEmbedder::AddBool(m_serverLuaState, "Client", false);
    }
}

void GameCreator::InitializeWorld(std::string _gameMode, WorldType _worldType, bool _isMainWorld, bool _includeMasterServer)
{
    lua_State* luaState = _worldType == WorldType::Client ? m_clientLuaState : m_serverLuaState;

	if (_worldType == WorldType::Server)
	{
		LoadingScreen::GetInstance().SetLoadingText("Loading server world.");
		//m_graphics->Render();
		std::vector<std::string> paths = HomePath::GetGameModePaths(HomePath::Type::Server);

		for (int i = 0; i < paths.size(); ++i)
		{
			std::vector<std::string> files = FileSystem::Directory::GetAllFiles(paths[i]);

			for (int j = 0; j < files.size(); ++j)
			{
				ResourceManager::AddGamemodeResource(files[j]);
			}
		}
	}
	else
	{
		LoadingScreen::GetInstance().SetLoadingText("Loading client world.");
		//m_graphics->Render();
	}
    
	LuaBridge::LuaWorldCreator worldCreator = LuaBridge::LuaWorldCreator(luaState);
    worldCreator.SkipComponentTypesAndTemplates(!_isMainWorld);
	LuaEmbedder::AddObject<LuaBridge::LuaWorldCreator>(luaState, "WorldCreator", &worldCreator, "worldCreator");

	std::vector<std::string> paths;
	if (NetworkInstance::GetServer()->IsRunning() || _gameMode == "lobbythreaded")
		paths = HomePath::GetGameModePaths(HomePath::Type::Server);
	else
		paths = HomePath::GetGameModePaths(HomePath::Type::Client);

	if (!LuaEmbedder::Load(luaState, &paths, "init.lua"))
		return;
	
	std::vector<ECSL::SystemWorkGroup*>* systemWorkGroups = worldCreator.GetSystemWorkGroups();
	unsigned int maxNoSystemsInWorkGroup = 0;
	for (ECSL::SystemWorkGroup* systemWorkGroup : *systemWorkGroups)
	{
		unsigned int systemCount = systemWorkGroup->GetSystems()->size();
		if (systemCount > maxNoSystemsInWorkGroup)
			maxNoSystemsInWorkGroup = systemCount;
	}
	for (unsigned int i = 0; i < maxNoSystemsInWorkGroup; ++i)
	{
		lua_State* luaStateCopy = LuaEmbedder::CreateChildState(luaState);
		LuaBridge::Embed(luaStateCopy);
		for (ECSL::SystemWorkGroup* systemWorkGroup : *systemWorkGroups)
		{
			if (i < systemWorkGroup->GetSystems()->size())
			{
				LuaBridge::LuaSystem* system = dynamic_cast<LuaBridge::LuaSystem*>(systemWorkGroup->GetSystems()->at(i));
				LuaEmbedder::CopyObject<LuaBridge::LuaSystem>(luaState, luaStateCopy, "System", system);
				system->SetLuaState(luaStateCopy);
			}
		}
	}

    
    if (_worldType == WorldType::Client)
    {
        LuaEmbedder::AddBool(m_clientLuaState, "Server", false);
        LuaEmbedder::AddBool(m_clientLuaState, "Client", true);
    }
    else
    {
        LuaEmbedder::AddBool(m_serverLuaState, "Server", true);
        LuaEmbedder::AddBool(m_serverLuaState, "Client", false);
    }
    
	LuaBridge::LuaGraphicDevice::SetGraphicDevice(m_graphics);
    LuaBridge::SetIOLuaState(m_clientLuaState);
    
    LuaBridge::LuaNetwork::SetClientLuaState(m_clientLuaState);
    LuaBridge::LuaNetwork::SetServerLuaState(m_serverLuaState);
    
    
	m_gameMode = _gameMode;

	auto componentTypes = ECSL::ComponentTypeManager::GetInstance().GetComponentTypes();
	for (auto it = componentTypes->begin(); it != componentTypes->end(); ++it)
	{
		worldCreator.AddComponentType(it->second->GetName());
		int id = ECSL::ComponentTypeManager::GetInstance().GetTableId(it->second->GetName());
		printf("[#%d] %s\n", id, it->second->GetName().c_str());
	}

	/*	This component has to be added last!	*/
	unsigned int numberOfComponents = ECSL::ComponentTypeManager::GetInstance().GetComponentTypeCount() + 2;
	unsigned int numberOfInts = ECSL::BitSet::GetDataTypeCount(numberOfComponents);
	unsigned int numberOfBytes = numberOfInts*sizeof(ECSL::BitSet::DataType);
	std::map<std::string, ECSL::ComponentVariable> m_variables;
	ECSL::ComponentVariable start = ECSL::ComponentVariable("ChangedComponents", numberOfBytes);
	m_variables.insert(std::pair<std::string, ECSL::ComponentVariable>("ChangedComponents", start));
	std::map<unsigned int, ECSL::ComponentDataType> m_offsetToType;
	m_offsetToType[0] = ECSL::ComponentDataType::INT64;

    
    if (_isMainWorld)
    {
        ECSL::ComponentType* changedComponents = new ECSL::ComponentType("ChangedComponents", ECSL::TableType::Array, m_variables, m_offsetToType, false);
        ECSL::ComponentTypeManager::GetInstance().AddComponentType(*changedComponents);
        worldCreator.AddComponentType("ChangedComponents");

        ECSL::ComponentType* changedComponentsNetwork = new ECSL::ComponentType("ChangedComponentsNetwork", ECSL::TableType::Array, m_variables, m_offsetToType,    false);
        ECSL::ComponentTypeManager::GetInstance().AddComponentType(*changedComponentsNetwork);
        worldCreator.AddComponentType("ChangedComponentsNetwork");
    }
	//NetworkMessagesSystem* nms = new NetworkMessagesSystem();
	//nms->SetConsole(&m_consoleManager);
	
	GraphicalSystem* graphicalSystem = 0;
	graphicalSystem = new GenerateIslandSystem(m_graphics, _worldType == WorldType::Client);
	m_graphicalSystems.push_back(graphicalSystem);
	worldCreator.AddSystemGroup();
	worldCreator.AddLuaSystemToCurrentGroup(graphicalSystem);

	graphicalSystem = new PointlightSystem(m_graphics);
	m_graphicalSystems.push_back(graphicalSystem);
	
	worldCreator.AddSystemGroup();
	worldCreator.AddLuaSystemToCurrentGroup(new SlerpRotationSystem());
	worldCreator.AddLuaSystemToCurrentGroup(graphicalSystem);

	graphicalSystem = new DirectionalLightSystem(m_graphics);
	m_graphicalSystems.push_back(graphicalSystem);
	worldCreator.AddLuaSystemToCurrentGroup(graphicalSystem);
    
    if (_worldType == WorldType::Client || _isMainWorld)
    {
        graphicalSystem = new CameraSystem(m_graphics);
        m_graphicalSystems.push_back(graphicalSystem);
        worldCreator.AddLuaSystemToCurrentGroup(graphicalSystem);
        
		graphicalSystem = new ModelSystem(m_graphics, _worldType == WorldType::Client);
        m_graphicalSystems.push_back(graphicalSystem);
        worldCreator.AddLuaSystemToCurrentGroup(graphicalSystem);

		//graphicalSystem = new AModelSystem(m_graphics);
		//m_graphicalSystems.push_back(graphicalSystem);
		//worldCreator.AddLuaSystemToCurrentGroup(graphicalSystem);
    }
    
    if (_worldType == WorldType::Client || _isMainWorld)
    {
		graphicalSystem = new AddTextToTextureSystem(m_graphics);
		m_graphicalSystems.push_back(graphicalSystem);
		worldCreator.AddSystemGroup();
		worldCreator.AddLuaSystemToCurrentGroup(graphicalSystem);
		worldCreator.AddSystemToCurrentGroup<SpinSystem>();
    }    

    if (_includeMasterServer)
    {
       worldCreator.AddSystemGroup();
       worldCreator.AddSystemToCurrentGroup<MasterServerSystem>();
    }
    
    if (_worldType == WorldType::Server)
    {
        worldCreator.AddSystemGroup();
        worldCreator.AddSystemToCurrentGroup<SyncEntitiesSystem>();
		//worldCreator.AddSystemToCurrentGroup<SimulateFrameSpikeSystem>();
    }
    
	//worldCreator.AddLuaSystemToCurrentGroup(new ReceivePacketSystem());
    
    if (_worldType == WorldType::Client || _isMainWorld)
    {
        graphicalSystem = new RenderSystem(m_graphics);
        m_graphicalSystems.push_back(graphicalSystem);
        worldCreator.AddSystemGroup();
        worldCreator.AddLuaSystemToCurrentGroup(graphicalSystem);
        //worldCreator.AddLuaSystemToCurrentGroup(new ReconnectSystem());
        graphicalSystem = new RenderRemoveSystem(m_graphics);
        m_graphicalSystems.push_back(graphicalSystem);
        worldCreator.AddSystemGroup();
        worldCreator.AddLuaSystemToCurrentGroup(graphicalSystem);
		graphicalSystem = new ParticleSystem(m_graphics);
		m_graphicalSystems.push_back(graphicalSystem);
		worldCreator.AddSystemGroup();
		worldCreator.AddLuaSystemToCurrentGroup(graphicalSystem);
        //worldCreator.AddSystemGroup();
        worldCreator.AddSystemToCurrentGroup<ResetChangedSystem>();
    }
    



	m_entityCount = worldCreator.GetMaxNumberOfEntities();
    if (_worldType == WorldType::Client)
    {
		m_clientWorld = worldCreator.CreateWorld(worldCreator.GetMaxNumberOfEntities());
		LuaEmbedder::CollectGarbageFull();
        LuaEmbedder::AddObject<ECSL::World>(luaState, "World", m_clientWorld, "world");
        m_clientWorld->PostInitializeSystems();
		LuaEmbedder::CollectGarbageFull();
    }
    else
    {
		m_serverWorld = worldCreator.CreateWorld(worldCreator.GetMaxNumberOfEntities());
		LuaEmbedder::CollectGarbageFull();
        LuaEmbedder::AddObject<ECSL::World>(luaState, "World", m_serverWorld, "world");
        m_serverWorld->PostInitializeSystems();
		LuaEmbedder::CollectGarbageFull();
    }

	if (!m_clientWorldProfiler)
		m_clientWorldProfiler = new Profilers::ECSLProfiler(m_graphics);
	if (!m_serverWorldProfiler)
		m_serverWorldProfiler = new Profilers::ECSLProfiler(m_graphics);
}

void GameCreator::RunStartupCommands(int argc, char** argv)
{
	for (int i = 1; i < argc; ++i)
	{
		if (argv[i][0] == '-')
		{
			int start = i;
			int stop = i;

			for (++i; i < argc; ++i)
			{
				if (argv[i][0] == '-')
				{
					--i;
					break;
				}
				stop = i;
			}

			int size = 0;

			for (int j = start; j <= stop; ++j)
			{
				size += strlen(argv[j]);
			}

			size += stop - start;

			char* command = new char[size];
			memcpy(command, argv[start] + 1, strlen(argv[start]) - 1);
			int offset = strlen(argv[start]);
			command[offset - 1] = ' ';
			for (int j = start + 1; j <= stop; ++j)
			{
				memcpy(command + offset, argv[j], strlen(argv[j]));
				offset += strlen(argv[j]);

				if (start < stop)
					command[offset] = ' ';
			}

			command[size - 1] = '\0';
			Console::ConsoleManager::GetInstance().AddToCommandQueue(command, false);
		}
	}
}

void noEffect(int chan, void *stream, int len, void *udata)
{
	float ratio = (22050 + 20000) / 22050.0f;
	short* samples = (short*)stream;
	int i = 0;
	for(float x = 0; x < len/2 - 1; x += ratio) {
		float p = x - int(x);
		samples[i++] = (1-p) * samples[int(x)] + p * samples[int(x) + 1];
	}

	for(; i < len/2; i++) {
		samples[i] = 0;
	}
}

void GameCreator::StartGame(int argc, char** argv)
{
	/*	If atleast one object is not initialized the game can't start	*/
	if (!m_graphics || !m_input)
		return;

	m_console = new GameConsole(m_graphics, m_serverWorld);

	m_consoleInput.SetTextHook(std::bind(&Console::ConsoleManager::AddToCommandQueue, &m_consoleManager, std::placeholders::_1, std::placeholders::_2));
	m_consoleInput.SetActive(false);
	m_input->GetKeyboard()->StopTextInput();

	/*	Hook console	*/
	m_console->SetupHooks(&m_consoleManager);
	m_consoleManager.AddCommand("Reload", std::bind(&GameCreator::ConsoleReload, this, std::placeholders::_1, std::placeholders::_2));
	m_consoleManager.AddCommand("Quit", std::bind(&GameCreator::ConsoleStopGame, this, std::placeholders::_1, std::placeholders::_2));
	m_consoleManager.AddCommand("GameMode", std::bind(&GameCreator::ConsoleGameMode, this, std::placeholders::_1, std::placeholders::_2));
	m_consoleManager.AddCommand("HostSettings", std::bind(&GameCreator::ConsoleHostSettings, this, std::placeholders::_1, std::placeholders::_2));
	m_consoleManager.AddCommand("Name", std::bind(&GameCreator::ConsoleName, this, std::placeholders::_1, std::placeholders::_2));
	m_consoleManager.AddCommand("Start", std::bind(&GameCreator::ConsoleStartTemp, this, std::placeholders::_1, std::placeholders::_2));
	m_consoleManager.AddCommand("ChangeGraphics", std::bind(&GameCreator::ChangeGraphicsSettings, this, std::placeholders::_1, std::placeholders::_2));
	m_consoleManager.AddCommand("TimeScale", std::bind(&GameCreator::ChangeTimeScale, this, std::placeholders::_1, std::placeholders::_2));
	
    InitializeLobby();
    
	RunStartupCommands(argc, argv);
    
    //Console::ConsoleManager::GetInstance().AddToCommandQueue("connect 192.168.0.198");
	/* The GraphicsDevice should probably do this instead. New: int refreshrate = m_graphics->GetRefreshRate() */
	int refreshRate;
	SDL_DisplayMode mode;
	if (SDL_GetDisplayMode(0, 0, &mode) == 0)
		refreshRate = std::max(mode.refresh_rate, 60);
	else
	{
		refreshRate = 60;
		std::string message = "Couldn't fetch the refresh rate. Max frame rate is now forced to 60.\n";
		message += SDL_GetError();
		Logger::GetInstance().Log("Refresh rate", LogSeverity::Warning, message);
	}
    
	float maxDeltaTime = (float)(1.0f / 20.0f);
	float minDeltaTime = (1.0f / (float)refreshRate);
	float bytesToMegaBytes = 1.f / (1024.f*1024.f);
	bool showDebugInfo = false;
	Utility::FrameCounter totalCounter;
	
	// Remove to enable audio
	Audio::SetVolume(128);

	while (m_running)
	{
        if (!(m_clientWorld || m_serverWorld) || !m_console)
        {
            break;
        }
        
		float dt = m_frameCounter->GetDeltaTime();

		/* Enforce max fps by looping until max fps is reached. Alternative solution is worth looking at */
		while (dt < minDeltaTime)
		{
			m_frameCounter->Tick();
			dt += m_frameCounter->GetDeltaTime();
		}

		dt *= m_timeScale;

		m_inputCounter.Reset();
		/*	Collect all input	*/
		m_input->Update();
		PollSDLEvent();
		m_consoleInput.Update();
		Console::ConsoleManager::GetInstance().ExecuteCommandQueue();
		UpdateConsole();
		if (m_input->GetKeyboard()->GetKeyState(SDL_SCANCODE_ESCAPE) == Input::InputState::PRESSED ||
		    m_input->GetKeyboard()->GetKeyState(SDL_SCANCODE_AC_BACK) == Input::InputState::PRESSED)
			break;
		m_inputCounter.Tick();

		m_serverWorldCounter.Reset();
		/*	Update world (systems, entities, etc)	*/
		

		m_serverWorldProfiler->Begin();
        if (m_serverWorld)
           m_serverWorld->Update(dt);
		m_serverWorldProfiler->End();
		m_serverWorldProfiler->Update(dt);
		m_serverWorldProfiler->Render();

		m_serverWorldCounter.Tick();
        
		m_clientWorldCounter.Reset();

		m_clientWorldProfiler->Begin();
        if (m_clientWorld)
            m_clientWorld->Update(dt);
		m_clientWorldProfiler->End();
		m_clientWorldProfiler->Update(dt);
		m_clientWorldProfiler->Render();
        
		m_clientWorldCounter.Tick();

		m_luaGarbageCollectionCounter.Reset();
		LuaEmbedder::CollectGarbageForDuration(0.1f * dt);
		m_luaGarbageCollectionCounter.Tick();
		
		Audio::SetCameraPosition(*m_graphics->GetCamera()->GetPos());
		Audio::Update();

		m_networkCounter.Reset();
		UpdateNetwork(dt);
		m_networkCounter.Tick();

		ClientManager::Update();

		/*	Update graphics	*/
		m_graphics->Update(dt);
		RenderConsole();
		m_graphicsCounter.Reset();
		m_graphics->Render();
		m_graphicsCounter.Tick();

		/*	DEBUG PRINT INFO	*/

		if (m_input->GetKeyboard()->GetKeyState(SDL_SCANCODE_F1) == Input::InputState::PRESSED)
		{
			if (m_clientWorldProfiler->IsActive())
				m_clientWorldProfiler->LogDisplayedStatistics();
			if (m_serverWorldProfiler->IsActive())
				m_serverWorldProfiler->LogDisplayedStatistics();
		}
		else if (m_input->GetKeyboard()->GetKeyState(SDL_SCANCODE_F2) == Input::InputState::PRESSED)
			showDebugInfo = !showDebugInfo;
		else if (m_input->GetKeyboard()->GetKeyState(SDL_SCANCODE_F3) == Input::InputState::PRESSED)
		{
			m_graphics->debugModelInfo = (m_graphics->debugModelInfo + 1) % 3;
		}
		else if (m_input->GetKeyboard()->GetKeyState(SDL_SCANCODE_F4) == Input::InputState::PRESSED)
		{
			if (!m_serverWorldProfiler->IsActive())
				m_clientWorldProfiler->Toggle();
		}
		else if (m_input->GetKeyboard()->GetKeyState(SDL_SCANCODE_F5) == Input::InputState::PRESSED)
		{
			if (!m_clientWorldProfiler->IsActive())
				m_serverWorldProfiler->Toggle();
		}
		else if (m_input->GetKeyboard()->GetKeyState(SDL_SCANCODE_F6) == Input::InputState::PRESSED)
		{
			if (m_clientWorldProfiler->IsActive())
				m_clientWorldProfiler->PreviousView();
			if (m_serverWorldProfiler->IsActive())
				m_serverWorldProfiler->PreviousView();
		}
		else if (m_input->GetKeyboard()->GetKeyState(SDL_SCANCODE_F7) == Input::InputState::PRESSED)
		{
			if (m_clientWorldProfiler->IsActive())
				m_clientWorldProfiler->NextView();
			if (m_serverWorldProfiler->IsActive())
				m_serverWorldProfiler->NextView();
		}
		else if (m_input->GetKeyboard()->GetKeyState(SDL_SCANCODE_F8) == Input::InputState::PRESSED)
		{
			m_graphics->hideInderface = !m_graphics->hideInderface;
		}

		if (showDebugInfo)
		{
			std::stringstream sstm;
			sstm << (int)(1 / dt) << " fps";
			m_graphics->RenderSimpleText(sstm.str(), 0, 0);

			std::stringstream vram;
			vram << "VRAM usage: " << (float)(m_graphics->GetVRamUsage()*bytesToMegaBytes) << " Mb ";
			m_graphics->RenderSimpleText(vram.str(), 20, 0);
			
			std::stringstream ss;
			ss << "Lua memory usage: " << LuaEmbedder::GetMemoryUsage() << " Kb";
			m_graphics->RenderSimpleText(ss.str(), 20, 1);

			std::stringstream slaves;
			slaves << "Slave Threads: " << MPL::TaskManager::GetInstance().GetSlaveCount();
			m_graphics->RenderSimpleText(slaves.str(), 20, 2);

			m_graphics->RenderSimpleText("Time Statistics", 60, 0);
			PrintSectionTime("Total       ", m_frameCounter, 60, 1);
			PrintSectionTime("Input       ", &m_inputCounter, 60, 2);
			PrintSectionTime("Server World", &m_serverWorldCounter, 60, 3);
			PrintSectionTime("Client World", &m_clientWorldCounter, 60, 4);
			PrintSectionTime("Network     ", &m_networkCounter, 60, 5);
			PrintSectionTime("Graphics    ", &m_graphicsCounter, 60, 6);
			PrintSectionTime("Lua GC      ", &m_luaGarbageCollectionCounter, 60, 7);
		}

		m_frameCounter->Tick();
	}
}

void GameCreator::UpdateNetwork(float _dt)
{
	Network::ServerNetwork* server = NetworkInstance::GetServer();
	Network::ClientNetwork* client = NetworkInstance::GetClient();

	bool serverRunning = server->IsRunning();
	bool clientConnected = client->IsConnected();

	if (serverRunning)
		server->Update(_dt);
	if (clientConnected)
		client->Update(_dt);

	if (serverRunning)
		while (server->PopAndExecutePacket() > 0) {}

	if (clientConnected)
		while (client->PopAndExecutePacket() > 0) {}
}

void GameCreator::GameMode(std::string _gamemode)
{
	if (_gamemode == "lobby")
		_gamemode = "lobbythreaded";
	m_gameMode = _gamemode;
	Reload();
}

void GameCreator::Reload()
{

    if (m_clientWorldProfiler)
    {
		delete m_clientWorldProfiler;
		m_clientWorldProfiler = nullptr;
    }

	if (m_serverWorldProfiler)
	{
		delete m_serverWorldProfiler;
		m_serverWorldProfiler = nullptr;
	}
    
    if (m_clientWorld)
    {
        delete m_clientWorld;
        m_clientWorld = nullptr;
    }
    
    if (m_serverWorld)
    {
        delete m_serverWorld;
        m_serverWorld = nullptr;
    }
	if (NetworkInstance::GetServer()->IsRunning())
	{
		ClientManager::SetAllClientsToConnecting();
	}

	if (NetworkInstance::GetServer()->IsRunning())
	{
		ClientManager::SetAllClientsToConnecting();
	}

	ResourceManager::Clear();
	
	HomePath::SetGameMode(m_gameMode);
		
	NetworkInstance::GetClientNetworkHelper()->ResetNetworkMaps();
    NetworkInstance::GetServerNetworkHelper()->ResetNetworkMaps();

	if (m_gameMode != "lobbythreaded" && m_gameMode != "loadingscreen")
		InitializeNetworkEvents(true);
	else
		InitializeNetworkEvents(false);
	//bool server = LuaEmbedder::PullBool(m_serverLuaState, "Server");
	//bool client = LuaEmbedder::PullBool(m_clientLuaState, "Client");
	LuaEmbedder::Quit();
	ECSL::ComponentTypeManager::GetInstance().Clear();
	ECSL::EntityTemplateManager::GetInstance().Clear();

    if (NetworkInstance::GetClient()->IsConnected() && NetworkInstance::GetServer()->IsRunning())
    {
        InitializeLua(WorldType::Client);
        InitializeLua(WorldType::Server);
    }
    
    else if (NetworkInstance::GetServer()->IsRunning())
    {
        InitializeLua(WorldType::Server);
    }
    
    else
    {
        InitializeLua(WorldType::Client);
    }
    
	//InitializeLua();

	m_graphics->Clear();

	LuaBridge::LuaGraphicDevice::SetGraphicDevice(m_graphics);

	if (NetworkInstance::GetServer()->IsRunning() || NetworkInstance::GetClient()->IsConnected())
	{
		LoadingScreen::GetInstance().SetActive();

		if (NetworkInstance::GetServer()->IsRunning())
		{
			std::vector<std::string> paths;
			paths.push_back(HomePath::GetSecondaryHomePath());
			std::vector<std::string> gmPaths = HomePath::GetGameModePaths(HomePath::Type::Server);

			for (int i = 0; i < gmPaths.size(); ++i)
				paths.push_back(gmPaths[i]);

			for (int i = 0; i < paths.size(); ++i)
			{
				std::string path = paths[i];
				path.append("loadingscreen.png");
				if (FileSystem::File::Exist(path))
				{
					LoadingScreen::GetInstance().SetBackground(path);
				}
			}
		}
	}

    if (!NetworkInstance::GetClient()->IsConnected() && NetworkInstance::GetServer()->IsRunning())
    {
        LuaBridge::SetIOLuaState(m_serverLuaState);
        LuaBridge::LuaEntityTemplateManager::SetLuaState(m_serverLuaState);
    }
    else
    {
        LuaBridge::SetIOLuaState(m_clientLuaState);
        LuaBridge::LuaEntityTemplateManager::SetLuaState(m_clientLuaState);
    }

    
    LuaBridge::LuaNetwork::SetClientLuaState(m_clientLuaState);
    LuaBridge::LuaNetwork::SetServerLuaState(m_serverLuaState);

	if (NetworkInstance::GetServer()->IsRunning())
	{
		LuaBridge::LuaResource::SetServerState(m_serverLuaState);
	}
	else
		LuaBridge::LuaResource::SetServerState(NULL);

	m_graphicalSystems.clear();

    if (NetworkInstance::GetClient()->IsConnected() && NetworkInstance::GetServer()->IsRunning())
    {
        InitializeWorld(m_gameMode, WorldType::Client, true, false);
		InitializeWorld(m_gameMode, WorldType::Server, false, true);
    }
    
    else if (NetworkInstance::GetServer()->IsRunning())
    {
        InitializeWorld(m_gameMode, WorldType::Server, true, true);
    }
    
    else
    {
        InitializeWorld(m_gameMode, WorldType::Client, true, true);
    }
    
	m_console->SetWorld(m_serverWorld);

	if (NetworkInstance::GetServer()->IsRunning())
	{
		Network::ServerNetwork* server = NetworkInstance::GetServer();
		Network::PacketHandler* ph = server->GetPacketHandler();
		uint64_t id = ph->StartPack("Gamemode");
		ph->WriteString(id, m_gameMode.c_str());
		NetworkInstance::GetServer()->Broadcast(ph->EndPack(id));
	}

	if (NetworkInstance::GetServer()->IsRunning() != NetworkInstance::GetClient()->IsConnected())
		LoadingScreen::GetInstance().SetInactive(0);
}

void GameCreator::LuaPacket(Network::PacketHandler* _ph, uint64_t& _id, Network::NetConnection& _nc)
{
	std::ostringstream ss;
	ss << _id;

	char* function = _ph->ReadString(_id);
    
    lua_State* luaState = LuaEmbedder::GetFunctionLuaState(function);
    
    if (luaState)
    {
        LuaEmbedder::PushString(luaState, ss.str());
        LuaEmbedder::PushString(luaState, _nc.GetIpAddress());
        LuaEmbedder::PushInt(luaState, (int)_nc.GetPort());
        LuaEmbedder::CallSavedFunction(function, 3);
    }
}

void GameCreator::NetworkGameMode(Network::PacketHandler* _ph, uint64_t& _id, Network::NetConnection& _nc)
{
	if (!NetworkInstance::GetServer()->IsRunning())
	{
		//GameMode("loadingscreen");
		ConnectHelper::Connect(_ph->ReadString(_id));
	}
	else
	{
        ConnectHelper::Connect(_ph->ReadString(_id));
		//Network::PacketHandler* ph = NetworkInstance::GetClient()->GetPacketHandler();
		//uint64_t id = ph->StartPack("GameModeLoaded");
		//NetworkInstance::GetClient()->Send(ph->EndPack(id));
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
			m_consoleManager.SetOpen(false);
		}
		else
		{
			m_input->GetKeyboard()->StartTextInput();
			m_consoleInput.SetActive(true);
			m_input->GetKeyboard()->ResetTextInput();
			m_consoleManager.SetOpen(true);
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
#ifdef __ANDROID__
			//exit(0);
#endif
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

void GameCreator::ConsoleReload(std::string _command, std::vector<Console::Argument>* _args)
{
    if (NetworkInstance::GetServer()->IsRunning())
    {
        Reload();

		unsigned int id = m_serverWorld->CreateNewEntity();
		m_serverWorld->CreateComponentAndAddTo("HostSettings", id);

		int port = NetworkInstance::GetServer()->GetIncomingPort();

		char* data = new char[m_name.size() + 1];
		memcpy(data, m_name.c_str(), m_name.size() + 1);
		m_serverWorld->SetComponent(id, "HostSettings", "Name", data);
		delete [](data);

		data = new char[m_map.size() + 1];
		memcpy(data, m_map.c_str(), m_map.size() + 1);
		m_serverWorld->SetComponent(id, "HostSettings", "Map", data);
		delete [](data);

		m_serverWorld->SetComponent(id, "HostSettings", "Port", &port);
		m_serverWorld->SetComponent(id, "HostSettings", "FillAI", &m_fillAI);
		m_serverWorld->SetComponent(id, "HostSettings", "AllowSpectators", &m_allowSpectators);

    }
}

void GameCreator::ConsoleStopGame(std::string _command, std::vector<Console::Argument>* _args)
{
	m_running = false;
}

void GameCreator::ConsoleGameMode(std::string _command, std::vector<Console::Argument>* _args)
{
	if (NetworkInstance::GetServer()->IsRunning() && NetworkInstance::GetClient()->IsConnected())
	{
		if (m_gameMode != "storaspelthreaded")
			GameMode("storaspelthreaded");
	}
    else if (NetworkInstance::GetServer()->IsRunning() || !NetworkInstance::GetClient()->IsConnected())
    {
        if (_args->size() == 0)
        {
            GameMode("storaspelthreaded");
        }
        else if (_args->size() == 1)
        {
            if (_args->at(0).ArgType == Console::ArgumentType::Text)
            {
                GameMode(_args->at(0).Text);
            }
        }
    }
	return;
}

void GameCreator::ConsoleHostSettings(std::string _command, std::vector<Console::Argument>* _args)
{

	m_name					= _args->at(0).Text;
	std::string password	= _args->at(1).Text;
	m_map					= _args->at(2).Text;
	std::string gamemode	= _args->at(3).Text;
	unsigned int port		= (unsigned int)_args->at(4).Number;
	m_fillAI				= _args->at(5).Number;
	m_allowSpectators		= _args->at(6).Number;
	int serverType			= _args->at(7).Number;

	if (NetworkInstance::GetClient()->IsConnected())
		NetworkInstance::GetClient()->Disconnect();
	if (NetworkInstance::GetServer()->IsRunning())
		NetworkInstance::GetServer()->Stop();

	unsigned int maxConnections = NetworkInstance::GetServer()->GetMaxConnections();

	if (!NetworkInstance::GetServer()->Start(port, password.c_str(), maxConnections))
	{
		NetworkInstance::GetServer()->Stop();
		return;
	}

	if (serverType == 0) // ListenServer
	{
		if (!NetworkInstance::GetClient()->Connect("127.0.0.1", password.c_str(), port, 0))
		{
			NetworkInstance::GetClient()->Disconnect();
			return;
		}
	}

	std::vector<Console::Argument> args;
	args.push_back(Console::Argument(gamemode.c_str()));
	ConsoleGameMode("gamemode", &args);

	unsigned int id = m_serverWorld->CreateNewEntity();
	m_serverWorld->CreateComponentAndAddTo("HostSettings", id);

	char* data = new char[m_name.size() + 1];
	memcpy(data, m_name.c_str(), m_name.size() + 1);
	m_serverWorld->SetComponent(id, "HostSettings", "Name", data);
	delete [] data;
	
	data = new char[m_map.size() + 1];
	memcpy(data, m_map.c_str(), m_map.size() + 1);
	m_serverWorld->SetComponent(id, "HostSettings", "Map", data);
	delete [] data;

	m_serverWorld->SetComponent(id, "HostSettings", "Port", &port);
	m_serverWorld->SetComponent(id, "HostSettings", "FillAI", &m_fillAI);
	m_serverWorld->SetComponent(id, "HostSettings", "AllowSpectators", &m_allowSpectators);
	m_serverWorld->SetComponent(id, "HostSettings", "ServerType", &serverType);


	//gamemode.insert(0, std::string("gamemode "));
	//m_consoleManager->AddToCommandQueue(gamemode.c_str());

}

void GameCreator::ConsoleName(std::string _command, std::vector<Console::Argument>* _args)
{
	if (_args->size() > 0)
	{
		std::string name = "";
		if (_args->at(0).ArgType == Console::ArgumentType::Text)
			name = _args->at(0).Text;
		else
			name = _args->at(0).Number;

		ConnectHelper::SetName(name.c_str());

		if (NetworkInstance::GetClient()->IsConnected())
		{
			auto ph = NetworkInstance::GetClient()->GetPacketHandler();
			auto id = ph->StartPack("LuaPacket");
			ph->WriteString(id, "CLIENT_SET_NAME");
			ph->WriteString(id, name.c_str());
			NetworkInstance::GetClient()->Send(ph->EndPack(id));
		}


	}
}

void GameCreator::ConsoleStartTemp(std::string _command, std::vector<Console::Argument>* _args)
{

}

void GameCreator::PrintSectionTime(const std::string& sectionName, Utility::FrameCounter* frameCounter, int x, int y)
{
	float average = 1000.f * frameCounter->GetAverageDeltaTime();
	float min = 1000.0f * frameCounter->GetMinDeltaTime();
	float max = 1000.0f * frameCounter->GetMaxDeltaTime();
	float anomality = 0.5f * ((average - min) + (max - average));
	
	std::stringstream ss;
	ss << sectionName <<
	  "   Average: " << std::fixed << std::setprecision(3) << average << " ms" <<
	  "   Anomality: " << std::fixed << std::setprecision(3) << anomality << " ms";
	m_graphics->RenderSimpleText(ss.str(), x, y);
}

void GameCreator::ChangeTimeScale(std::string _command, std::vector<Console::Argument>* _args)
{
	if (_args->size() == 0)
	{
		m_timeScale = 1.0f;
		return;
	}

	if (_args->at(0).ArgType == Console::ArgumentType::Number)
		m_timeScale = (float)abs((float)_args->at(0).Number);



}

void GameCreator::ChangeGraphicsSettings(std::string _command, std::vector<Console::Argument>* _args)
{
	if (_args->size() == 0)
		return;

#if defined(__IOS__) || defined(__ANDROID__)
	
	if ((*_args)[0].ArgType == Console::ArgumentType::Text)
	{
		for (int i = 0; i < m_entityCount; ++i)
		{
			if (m_clientWorld && m_clientWorld->HasComponent(i, "Render"))
				m_clientWorld->RemoveComponentFrom("Render", i);

			if (m_clientWorld && m_clientWorld->HasComponent(i, "Particle"))
				m_clientWorld->CreateComponentAndAddTo("Hide", i);

			if (m_serverWorld && m_serverWorld->HasComponent(i, "Render"))
				m_serverWorld->RemoveComponentFrom("Render", i);
		}

		if (strcmp((*_args)[0].Text, "high") == 0)
		{
			m_graphics->Clear();
			Camera tmpCam = *m_graphics->GetCamera();
			delete(m_graphics);

			m_graphics = new Renderer::GraphicsHigh(tmpCam);
			m_graphics->Init();
		}

		else if (strcmp((*_args)[0].Text, "low") == 0)
		{
			m_graphics->Clear();
			Camera tmpCam = *m_graphics->GetCamera();
			delete(m_graphics);

			m_graphics = new Renderer::GraphicsLow(tmpCam);
			m_graphics->Init();
		}
		if (m_input)
		{
			delete m_input;
			InitializeInput();
		}


		m_console->SetGraphicDevice(m_graphics);
		LuaBridge::LuaGraphicDevice::SetGraphicDevice(m_graphics);
		LoadingScreen::GetInstance().SetGraphicsDevice(m_graphics);
		for (int n = 0; n < m_graphicalSystems.size(); ++n)
		{
			GraphicalSystem* tSystem = m_graphicalSystems.at(n);
			tSystem->SetGraphics(m_graphics);
		}
	}
#endif

#if !defined(__IOS__) && !defined(__ANDROID__)

	if ((*_args)[0].ArgType == Console::ArgumentType::Text)
	{
		for (int i = 0; i < m_entityCount; ++i)
		{
			if (m_clientWorld && m_clientWorld->HasComponent(i, "Render"))
				m_clientWorld->RemoveComponentFrom("Render", i);

			if (m_clientWorld && m_clientWorld->HasComponent(i, "Particle"))
				m_clientWorld->CreateComponentAndAddTo("Hide", i);
            
			if (m_serverWorld && m_serverWorld->HasComponent(i, "Render"))
                m_serverWorld->RemoveComponentFrom("Render", i);
		}

		if (strcmp((*_args)[0].Text, "high") == 0)
		{
			m_graphics->Clear();
			Camera tmpCam = *m_graphics->GetCamera();
			int windowx, windowy;
			m_graphics->GetWindowPos(windowx, windowy);

		//	SDL_Window*	tmpWindow = m_graphics->GetSDL_Window();
		//	SDL_GLContext* tmpContext = m_graphics->GetSDL_GLContext();*/
			delete(m_graphics);

			m_graphics = new Renderer::GraphicsHigh(tmpCam, windowx, windowy);
			if (!m_graphics->Init())
			{
				SDL_Log("Switching to OpenGL 4.0");
				delete(m_graphics);
				m_graphics = new Renderer::GraphicsLow();
				m_graphics->Init();
			}
		}

		else if (strcmp((*_args)[0].Text, "low") == 0)
		{
			m_graphics->Clear();
			Camera tmpCam				= *m_graphics->GetCamera();
			int windowx, windowy;
			m_graphics->GetWindowPos(windowx, windowy);
			//SDL_Window*	tmpWindow		=  m_graphics->GetSDL_Window();
			//SDL_GLContext tmpContext	=  m_graphics->GetSDL_GLContext();


			delete(m_graphics);

			m_graphics = new Renderer::GraphicsLow(tmpCam, windowx, windowy);
			m_graphics->Init();
		}
		if (m_input)
		{
			delete m_input;
			InitializeInput();
		}

			

		m_console->SetGraphicDevice(m_graphics);
		
		LuaBridge::LuaGraphicDevice::SetGraphicDevice(m_graphics);
		LoadingScreen::GetInstance().SetGraphicsDevice(m_graphics);
		for (int n = 0; n < m_graphicalSystems.size(); ++n)
		{
			GraphicalSystem* tSystem = m_graphicalSystems.at(n);
			tSystem->SetGraphics(m_graphics);
		}
	}
#endif
}

void GameCreator::OnBannedFromServer(Network::NetConnection _nc, const char* _message)
{
	for (int i = 0; i < LuaBridge::LuaNetworkEvents::g_onBannedFromServer.size(); ++i)
	{
		LuaBridge::LuaNetworkEvents::g_onBannedFromServer[i](_nc, _message);
	}
}

void GameCreator::OnConnectedToServer(Network::NetConnection _nc, const char* _message)
{
	for (int i = 0; i < LuaBridge::LuaNetworkEvents::g_onConnectedToServer.size(); ++i)
	{
		LuaBridge::LuaNetworkEvents::g_onConnectedToServer[i](_nc, _message);
	}
}

void GameCreator::OnDisconnectedFromServer(Network::NetConnection _nc, const char* _message)
{
	for (int i = 0; i < LuaBridge::LuaNetworkEvents::g_onDisconnectedFromServer.size(); ++i)
	{
		LuaBridge::LuaNetworkEvents::g_onDisconnectedFromServer[i](_nc, _message);
	}
}
void GameCreator::OnFailedToConnect(Network::NetConnection _nc, const char* _message)
{
	for (int i = 0; i < LuaBridge::LuaNetworkEvents::g_onFailedToConnect.size(); ++i)
	{
		LuaBridge::LuaNetworkEvents::g_onFailedToConnect[i](_nc, _message);
	}
}
void GameCreator::OnKickedFromServer(Network::NetConnection _nc, const char* _message)
{
	for (int i = 0; i < LuaBridge::LuaNetworkEvents::g_onKickedFromServer.size(); ++i)
	{
		LuaBridge::LuaNetworkEvents::g_onKickedFromServer[i](_nc, _message);
	}
}
void GameCreator::OnPasswordInvalid(Network::NetConnection _nc, const char* _message)
{
	for (int i = 0; i < LuaBridge::LuaNetworkEvents::g_onPasswordInvalid.size(); ++i)
	{
		LuaBridge::LuaNetworkEvents::g_onPasswordInvalid[i](_nc, _message);
	}
}
void GameCreator::OnRemotePlayerBanned(Network::NetConnection _nc, const char* _message)
{
	for (int i = 0; i < LuaBridge::LuaNetworkEvents::g_onRemotePlayerBanned.size(); ++i)
	{
		LuaBridge::LuaNetworkEvents::g_onRemotePlayerBanned[i](_nc, _message);
	}
}
void GameCreator::OnRemotePlayerConnected(Network::NetConnection _nc, const char* _message)
{
	for (int i = 0; i < LuaBridge::LuaNetworkEvents::g_onRemotePlayerConnected.size(); ++i)
	{
		LuaBridge::LuaNetworkEvents::g_onRemotePlayerConnected[i](_nc, _message);
	}
}
void GameCreator::OnRemotePlayerDisconnected(Network::NetConnection _nc, const char* _message)
{
	for (int i = 0; i < LuaBridge::LuaNetworkEvents::g_onRemotePlayerDisconnected.size(); ++i)
	{
		LuaBridge::LuaNetworkEvents::g_onRemotePlayerDisconnected[i](_nc, _message);
	}
}
void GameCreator::OnRemotePlayerKicked(Network::NetConnection _nc, const char* _message)
{
	for (int i = 0; i < LuaBridge::LuaNetworkEvents::g_onRemotePlayerKicked.size(); ++i)
	{
		LuaBridge::LuaNetworkEvents::g_onRemotePlayerKicked[i](_nc, _message);
	}
}
void GameCreator::OnServerFull(Network::NetConnection _nc, const char* _message)
{
	for (int i = 0; i < LuaBridge::LuaNetworkEvents::g_onServerFull.size(); ++i)
	{
		LuaBridge::LuaNetworkEvents::g_onServerFull[i](_nc, _message);
	}
}

void GameCreator::OnTimedOutFromServer(Network::NetConnection _nc, const char* _message)
{
	for (int i = 0; i < LuaBridge::LuaNetworkEvents::g_onTimedOutFromServer.size(); ++i)
	{
		LuaBridge::LuaNetworkEvents::g_onTimedOutFromServer[i](_nc, _message);
	}
}

void GameCreator::OnPlayerConnected(Network::NetConnection _nc, const char* _message)
{
	ClientManager::NewClient(_nc);

	Network::ServerNetwork* server = NetworkInstance::GetServer();
	Network::PacketHandler* ph = server->GetPacketHandler();


	//Send gamemode
	uint64_t id = ph->StartPack("Gamemode");
	ph->WriteString(id, m_gameMode.c_str());
	NetworkInstance::GetServer()->Send(ph->EndPack(id), _nc);

}
void GameCreator::OnPlayerDisconnected(Network::NetConnection _nc, const char* _message)
{
	ClientManager::ClientDisconnected(_nc);
	for (int i = 0; i < LuaBridge::LuaNetworkEvents::g_onPlayerDisconnected.size(); ++i)
	{
		LuaBridge::LuaNetworkEvents::g_onPlayerDisconnected[i](_nc, _message);
	}
}
void GameCreator::OnPlayerTimedOut(Network::NetConnection _nc, const char* _message)
{
	ClientManager::ClientDisconnected(_nc);
	for (int i = 0; i < LuaBridge::LuaNetworkEvents::g_onPlayerTimedOut.size(); ++i)
	{
		LuaBridge::LuaNetworkEvents::g_onPlayerTimedOut[i](_nc, _message);
	}
}
