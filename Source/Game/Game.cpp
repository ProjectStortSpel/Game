#include <SDL/SDL.h>
#include <ECSL/ECSL.h>
#include "Input/InputWrapper.h"
#include "Renderer/GraphicDevice.h"
#include "Network/ServerNetwork.h"
#include "Network/ClientNetwork.h"
#include "Console/Console.h"
#include "Timer.h"
#include "Game/TextInput.h"
#include "Game/RemoteConsole.h"
#include "NetworkInstance.h"

#include <stdio.h>

#ifdef WIN32
	#include <stdlib.h>
	#include <crtdbg.h>
	#include <VLD/vld.h>
#endif

using namespace ECSL;
using namespace Console;

struct Player
{
	unsigned short Id = 0;
	bool Connected = false;
};

mat4 mat[1000];
RemoteConsole remoteConsole;
//Console::ConsoleManager consoleManager;
bool isServer = false;
bool isClient = false;
std::string newPlayer = "";
std::map<Network::NetConnection, Player*> m_players;



class TestSystem : public ECSL::System
{
public:
	TestSystem() { }
	~TestSystem() { }

	void Run(float _dt)
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

	void Run(float _dt)
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

void ClearConsole()
{
#ifdef WIN32
	system("cls");
#else
	system("clear");
#endif
}
void lol2()
{
	ComponentTypeManager::GetInstance().LoadComponentTypesFromDirectory("content/components");
	ECSL::WorldCreator worldCreator = ECSL::WorldCreator();
	worldCreator.AddSystemGroup();
	worldCreator.AddSystemToCurrentGroup<TestSystem2>();
	//worldCreator.AddLuaSystemToCurrentGroup(new TestSystem());
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
			mat[y+x*10] = glm::translate(vec3(x - 5, -1, y - 5));
			r->LoadModel("content/models/default_tile/", "default.object", &mat[y + x * 10]);
		}
	}
}
void OnConnected(Network::NetConnection nc)
{
	std::stringstream ss;
	ss << "connected to server " << nc.GetIpAddress() << ":" << nc.GetPort() << ".\n";
	newPlayer = ss.str();
}

void OnPlayerConnected(Network::NetConnection _nc)
{
	if (m_players.find(_nc) == m_players.end())
	{
		m_players[_nc] = new Player();
		m_players[_nc]->Connected = true;
		m_players[_nc]->Id = m_players.size();
	}

	Network::PacketHandler* ph = NetworkInstance::GetServer()->GetPacketHandler();
	uint64_t id = ph->StartPack("CubePos");
	ph->WriteFloat(id, mat[100][3][0]);
	ph->WriteFloat(id, mat[100][3][1]);
	ph->WriteFloat(id, mat[100][3][2]);
	Network::Packet* p = ph->EndPack(id);
	NetworkInstance::GetServer()->Send(p, _nc);


	std::stringstream ss;
	ss << _nc.GetIpAddress() << ":" << _nc.GetPort() << " connected to server.\n";
	newPlayer = ss.str();
}

void OnPlayerDisconnected(Network::NetConnection _nc)
{
	m_players[_nc]->Connected = false;
}

void OnKicked(Network::NetConnection _nc)
{
	NetworkInstance::GetClient()->Disconnect();
	printf("I was kicked from the server!\n");
}

void OnRemotePlayerKicked(Network::NetConnection _nc)
{
	printf("%s:%d was kicked from the server.\n", _nc.GetIpAddress(), _nc.GetPort());
}

void CubePos(Network::PacketHandler* _ph, uint64_t _id, Network::NetConnection _nc)
{
	mat[100][3][0] = _ph->ReadFloat(_id);
	mat[100][3][1] = _ph->ReadFloat(_id);
	mat[100][3][2] = _ph->ReadFloat(_id);
}

void KickPlayer(std::vector<Argument>* _args)
{
	if (m_players.size() == 0)
		return;

	auto firstplayer = m_players.begin();
	NetworkInstance::GetServer()->Kick(firstplayer->first, "Why not?");
	m_players.erase(firstplayer->first);
}

void MoveCube(std::vector<Argument>* _vec)
{
	if (_vec->size() != 3)
		return;

	for (int i = 0; i < 3; ++i)
	{
		if (_vec->at(i).ArgType != ArgumentType::Number)
			return;
	}

	mat[100][3][0] = _vec->at(0).Number;
	mat[100][3][1] = _vec->at(1).Number;
	mat[100][3][2] = _vec->at(2).Number;

	Network::PacketHandler* ph = NetworkInstance::GetServer()->GetPacketHandler();
	uint64_t id = ph->StartPack("CubePos");
	ph->WriteFloat(id, mat[100][3][0]);
	ph->WriteFloat(id, mat[100][3][1]);
	ph->WriteFloat(id, mat[100][3][2]);
	Network::Packet* p = ph->EndPack(id);
	NetworkInstance::GetServer()->Broadcast(p);
}

void Disconnect()
{
	isClient = false;
	remoteConsole.UpdateCommands();
	if (NetworkInstance::GetClient())
		NetworkInstance::GetClient()->Disconnect();
}

void OnDisconnected(Network::NetConnection nc)
{
	isClient = false;
	remoteConsole.UpdateCommands();
}

void Stop()
{
	Console::ConsoleManager::GetInstance().RemoveCommand("movecube");
	Console::ConsoleManager::GetInstance().RemoveCommand("kick");
	isServer = false;
	remoteConsole.UpdateCommands();
	if (NetworkInstance::GetServer())
		NetworkInstance::GetServer()->Stop();
}

bool Host(short _port)
{
	if (!isServer)
	{
		if (isClient)
		{
			Disconnect();
		}
		NetworkInstance::InitServer();
		NetworkInstance::GetServer()->SetOnPlayerConnected(&OnPlayerConnected);
		NetworkInstance::GetServer()->SetOnPlayerDisconnected(&OnPlayerDisconnected);
		NetworkInstance::GetServer()->SetOnPlayerTimedOut(&OnPlayerDisconnected);
		if (NetworkInstance::GetServer()->Start(_port, "localhest", 8))
		{
			remoteConsole.UpdateCommands();
			Console::ConsoleManager::GetInstance().AddCommand("movecube", &MoveCube);
			Console::ConsoleManager::GetInstance().AddCommand("kick", &KickPlayer);
			isServer = true;
			return true;
		}
		else
		{
			NetworkInstance::DestroyServer();
		}
	}
	return false;
}

void Stop_Hook(std::vector<Argument>* _vec)
{
	Stop();
}


bool Connect(char* _ip, short _port)
{
	if (!isClient)
	{
		if (isServer)
		{
			Stop();
		}
		NetworkInstance::InitClient();
		NetworkInstance::GetClient()->AddNetworkHook("CubePos", &CubePos);
		NetworkInstance::GetClient()->SetOnConnectedToServer(&OnConnected);
		NetworkInstance::GetClient()->SetOnDisconnectedFromServer(&OnDisconnected);
		NetworkInstance::GetClient()->SetOnKickedFromServer(&OnKicked);
		NetworkInstance::GetClient()->SetOnRemotePlayerKicked(&OnRemotePlayerKicked);
		if (NetworkInstance::GetClient()->Connect(_ip, "localhest", _port, 0))
		{
			remoteConsole.UpdateCommands();
			isClient = true;
			return true;
		}
		else
		{
			NetworkInstance::DestroyClient();
		}
	}
	return false;
}

void Disconnect_Hook(std::vector<Argument>* _vec)
{
	Disconnect();
}

void Connect_Hook(std::vector<Argument>* _vec)
{
	if (_vec->size() == 0)
		Connect("127.0.0.1", 6112);

	if (_vec->size() == 1)
	{
		if (_vec->at(0).ArgType == ArgumentType::Text)
			Connect((char*)_vec->at(0).Text, 6112);
	}

	if (_vec->size() == 2)
	{
		if (_vec->at(0).ArgType == ArgumentType::Text && _vec->at(1).ArgType == ArgumentType::Number)
			Connect((char*)_vec->at(0).Text, (short)_vec->at(1).Number);
	}
	
}

void Host_Hook(std::vector<Argument>* _vec)
{
	if (_vec->size() == 0)
		Host(6112);
	else if (_vec->size() == 1)
	{
		if (_vec->at(0).ArgType == ArgumentType::Number)
			Host((short)_vec->at(0).Number);
	}
}

bool lol;

void Quit_Hook(std::vector<Argument>* _vec)
{
	lol = false;
}

void Start()
{
	std::string input;

	Console::ConsoleManager* CONSOLE = &Console::ConsoleManager::GetInstance();

	CONSOLE->AddCommand("connect", &Connect_Hook);
	CONSOLE->AddCommand("disconnect", &Disconnect_Hook);
	CONSOLE->AddCommand("host", &Host_Hook);
	CONSOLE->AddCommand("stop", &Stop_Hook);
	CONSOLE->AddCommand("quit", &Quit_Hook);

	Renderer::GraphicDevice RENDERER = Renderer::GraphicDevice();
	/*	Initialize Renderer and Input	*/
	RENDERER.Init();

	Input::InputWrapper* INPUT = &Input::InputWrapper::GetInstance();

	LoadAlotOfBoxes(&RENDERER);
	mat[100] = glm::translate(vec3(0, 0, 0));
	int modelid = RENDERER.LoadModel("content/models/gamebrick/", "spelpjaas.object", &mat[100]); // LOADMODEL RETURNS THE MODELID

	lol = true;
	lol2();
	float cd = 1.0f;
	Timer timer;

	TextInput ti;
	ti.SetTextHook(std::bind(&Console::ConsoleManager::ExecuteCommand, CONSOLE, std::placeholders::_1));
	ti.SetActive(true);

	while (lol)
	{
		// DT COUNTER
		float dt = timer.ElapsedTimeInSeconds();
		timer.Reset();

		float tBytesReceived = 0;
		float tBytesSent = 0;

		float cBytesReceived = 0;
		float cBytesSent = 0;

		float ping = 0;

		if (isServer)
		{
			NetworkInstance::GetServer()->Update(dt);
			while (NetworkInstance::GetServer()->TriggerPacket() > 0) {}

			tBytesReceived = NetworkInstance::GetServer()->GetTotalBytesReceived() / 1024;
			tBytesSent = NetworkInstance::GetServer()->GetTotalBytesSent() / 1024;

			cBytesReceived = NetworkInstance::GetServer()->GetCurrentBytesReceived() / 1024;
			cBytesSent = NetworkInstance::GetServer()->GetCurrentBytesSent() / 1024;

		}

		if (isClient)
		{
			NetworkInstance::GetClient()->Update(dt);
			while (NetworkInstance::GetClient()->TriggerPacket() > 0) {}

			tBytesReceived = NetworkInstance::GetClient()->GetTotalBytesReceived() / 1024;
			tBytesSent = NetworkInstance::GetClient()->GetTotalBytesSent() / 1024;

			cBytesReceived = NetworkInstance::GetClient()->GetCurrentBytesReceived() / 1024;
			cBytesSent = NetworkInstance::GetClient()->GetCurrentBytesSent() / 1024;

			ping = NetworkInstance::GetClient()->GetPing();
		}

		INPUT->Update();
		RENDERER.Update(dt);


		char buffer[256];
#ifdef WIN32
		sprintf_s(buffer, "Network usage:\nPing: %1.0fms\nTotal received: %1.2f Kb\nTotal sent: %1.2f Kb\nCurrent received: %f Kb\nCurrent sent: %f Kb", ping, tBytesReceived, tBytesSent, cBytesReceived, cBytesSent);
#else
		sprintf(buffer, "Network usage:\nPing: %1.0fmsTotal received: %1.2f Kb\nTotal sent: %1.2f Kb\nCurrent received: %1.2f Kb\nCurrent sent: %1.2f Kb", ping, tBytesReceived, tBytesSent, cBytesReceived, cBytesSent);
#endif

		//std::string networkData = "Network usage:\nTotal received: " + std::to_string(tBytesReceived) + " Kb\nTotal sent: " + std::to_string(tBytesSent) + " Kb\nCurrent received: " + std::to_string(cBytesReceived) + " Kb\nCurrent sent: " + std::to_string(cBytesSent) + " Kb";
		RENDERER.RenderSimpleText(buffer, 0, 2);

		if (INPUT->GetKeyboard()->IsTextInputActive())
		{
			RENDERER.RenderSimpleText("Console:", 0, 9);
			RENDERER.RenderSimpleText(ti.GetText(), 0, 10);
		}

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
			case SDL_TEXTINPUT:
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

		ti.Update();
		//char* test = ti.GetText();
		
		if (!INPUT->GetKeyboard()->IsTextInputActive())
		{
			if (isServer)
			{
				bool cubeMoved = false;

				// MOVE CUBE
				if (INPUT->GetKeyboard()->GetKeyState(SDL_SCANCODE_UP) == Input::InputState::DOWN)
				{
					cubeMoved = true;
					mat[100] *= glm::translate(vec3(0, 0, -0.01f));
				}

				if (INPUT->GetKeyboard()->GetKeyState(SDL_SCANCODE_DOWN) == Input::InputState::DOWN)
				{
					cubeMoved = true;
					mat[100] *= glm::translate(vec3(0, 0, 0.01f));
				}

				if (INPUT->GetKeyboard()->GetKeyState(SDL_SCANCODE_LEFT) == Input::InputState::DOWN)
				{
					cubeMoved = true;
					mat[100] *= glm::translate(vec3(-0.01f, 0, 0));
				}

				if (INPUT->GetKeyboard()->GetKeyState(SDL_SCANCODE_RIGHT) == Input::InputState::DOWN)
				{
					cubeMoved = true;
					mat[100] *= glm::translate(vec3(0.01f, 0, 0));
				}

				if (INPUT->GetKeyboard()->GetKeyState(SDL_SCANCODE_SPACE) == Input::InputState::DOWN)
				{
					cubeMoved = true;
					mat[100] *= glm::translate(vec3(0, 0.01f, 0));
				}

				if (INPUT->GetKeyboard()->GetKeyState(SDL_SCANCODE_LSHIFT) == Input::InputState::DOWN)
				{
					cubeMoved = true;
					mat[100] *= glm::translate(vec3(0, -0.01f, 0));
				}

				if (cubeMoved)
				{
					Network::PacketHandler* ph = NetworkInstance::GetServer()->GetPacketHandler();
					uint64_t id = ph->StartPack("CubePos");
					ph->WriteFloat(id, mat[100][3][0]);
					ph->WriteFloat(id, mat[100][3][1]);
					ph->WriteFloat(id, mat[100][3][2]);
					Network::Packet* p = ph->EndPack(id);
					NetworkInstance::GetServer()->Broadcast(p);
				}
			}

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

		}

		// ToggleConsole
		if (INPUT->GetKeyboard()->GetKeyState(SDL_SCANCODE_F1) == Input::InputState::PRESSED)
		{
			if (INPUT->GetKeyboard()->IsTextInputActive())
			{
				INPUT->GetKeyboard()->StopTextInput();
				ti.SetActive(false);
			}
			else
			{
				INPUT->GetKeyboard()->StartTextInput();
				INPUT->GetKeyboard()->ResetTextInput();
				ti.SetActive(true);
			}
		}

		//-----------------------------------------------------------------------------------------------

		if (INPUT->GetKeyboard()->GetKeyState(SDL_SCANCODE_ESCAPE) == Input::InputState::PRESSED)
		{
			lol = false;
		}
	}

	for (auto it = m_players.begin(); it != m_players.end(); ++it)
		SAFE_DELETE(it->second);
	m_players.clear();



	SAFE_DELETE(INPUT);
	SAFE_DELETE(CONSOLE);
	NetworkInstance::DestroyServer();
	NetworkInstance::DestroyClient();
}

int main(int argc, char** argv)
{

	Start();
	return 0;
}