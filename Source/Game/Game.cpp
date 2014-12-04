#include <SDL/SDL.h>
#include <ECSL/ECSL.h>
#include "Input/InputWrapper.h"
#include "Renderer/GraphicDevice.h"
#include "Network/ServerNetwork.h"
#include "Network/ClientNetwork.h"
#include "Console/Console.h"
#include "Timer.h"

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
Network::ServerNetwork* server = 0;
Network::ClientNetwork* client = 0;
bool isServer = false;
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
void OnConnected(Network::NetConnection nc)
{
	std::stringstream ss;
	ss << "connected to server " << nc.IpAddress << ":" << nc.Port << ".\n";
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

	Network::PacketHandler* ph = server->GetPacketHandler();
	uint64_t id = ph->StartPack("CubePos");
	ph->WriteFloat(id, mat[100][3][0]);
	ph->WriteFloat(id, mat[100][3][1]);
	ph->WriteFloat(id, mat[100][3][2]);
	Network::Packet* p = ph->EndPack(id);
	server->Send(p, _nc);


	std::stringstream ss;
	ss << _nc.IpAddress << ":" << _nc.Port << " connected to server.\n";
	newPlayer = ss.str();
}

void OnPlayerDisconnected(Network::NetConnection _nc)
{
	m_players[_nc]->Connected = false;
}

void CubePos(Network::PacketHandler* _ph, uint64_t _id, Network::NetConnection _nc)
{
	mat[100][3][0] = _ph->ReadFloat(_id);
	mat[100][3][1] = _ph->ReadFloat(_id);
	mat[100][3][2] = _ph->ReadFloat(_id);
}


void first(std::vector<Argument>* _vec)
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

	Network::PacketHandler* ph = server->GetPacketHandler();
	uint64_t id = ph->StartPack("CubePos");
	ph->WriteFloat(id, mat[100][3][0]);
	ph->WriteFloat(id, mat[100][3][1]);
	ph->WriteFloat(id, mat[100][3][2]);
	Network::Packet* p = ph->EndPack(id);
	server->Broadcast(p);
}


void Start()
{
	std::string input;
	printf("Starting network:\n\n");
	printf("Press 's' to start a new server,\n");
	printf("c to connect to localhost,\n");
	printf("1 to connect to Jenkins Windows,\n");
	printf("2 to connect to Jenkins Linux,\n");
	printf("3 to connect to Jenkins Pontus,\n");
	printf("4 to connect to Jenkins Erik,\n");
	printf("or anything else to skip the network.\n");

	std::getline(std::cin, input);
	ClearConsole();


	if (input.compare("s") == 0)
	{
		isServer = true;
		server = new Network::ServerNetwork();
		server->Start(6112, "localhest", 8);
		server->SetOnPlayerConnected(&OnPlayerConnected);
		server->SetOnPlayerDisconnected(&OnPlayerDisconnected);
		server->SetOnPlayerTimedOut(&OnPlayerDisconnected);
	}
	else
	{
		client = new Network::ClientNetwork();
		client->AddNetworkHook("CubePos", &CubePos);
		client->SetOnConnectedToServer(&OnConnected);

		if (input.compare("c") == 0) // localhost
			client->Connect("127.0.0.1", "localhest", 6112, 0);
		else if (input.compare("1") == 0) // Jenkins win
			client->Connect("194.47.150.4", "localhest", 6112, 0);
		else if (input.compare("2") == 0) // Jenkins lin
			client->Connect("194.47.150.60", "localhest", 6112, 0);
		else if (input.compare("3") == 0) // Pontus
			client->Connect("83.233.56.219", "localhest", 6112, 0);
		else if (input.compare("4") == 0) // Erik
			client->Connect("194.47.150.5", "localhest", 6112, 0);
	}




	Renderer::GraphicDevice RENDERER = Renderer::GraphicDevice();
	/*	Initialize Renderer and Input	*/
	RENDERER.Init();

	Input::InputWrapper* INPUT = &Input::InputWrapper::GetInstance();

	LoadAlotOfBoxes(&RENDERER);
	mat[100] = glm::translate(vec3(0, 0, 0));
	int modelid = RENDERER.LoadModel("content/models/cube/", "cube.object", &mat[100]); // LOADMODEL RETURNS THE MODELID
	RENDERER.ChangeModelTexture(modelid, "content/models/cube/NM_tst.png"); // CHANGING TEXTURE ON MODELID

	bool lol = true;
	float cd = 1.0f;
	Timer timer;


	Console::ConsoleManager cm;
	if (isServer)
	{
		cm.AddCommand("first", &first);
		cm.ExecuteCommand("first 0 10 0");
	}
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
			server->Update(dt);
			while (server->TriggerPacket() > 0) {}

			tBytesReceived = server->GetTotalBytesReceived() * 0.001;
			tBytesSent = server->GetTotalBytesSent() * 0.001;

			cBytesReceived = server->GetCurrentBytesReceived() * 0.001;
			cBytesSent = server->GetCurrentBytesSent() * 0.001;

		}
		else
		{
			client->Update(dt);
			while (client->TriggerPacket() > 0) {}

			tBytesReceived = client->GetTotalBytesReceived() * 0.001;
			tBytesSent = client->GetTotalBytesSent() * 0.001;

			cBytesReceived = client->GetCurrentBytesReceived() * 0.001;
			cBytesSent = client->GetCurrentBytesSent() * 0.001;

			ping = client->GetPing();
		}

		INPUT->Update();
		RENDERER.Update(dt);


		char buffer[125];
#ifdef WIN32
		sprintf_s(buffer, "Network usage:\nPing: %1.0fms\nTotal received: %1.2f Kb\nTotal sent: %1.2f Kb\nCurrent received: %1.2f Kb\nCurrent sent: %1.2f Kb", ping, tBytesReceived, tBytesSent, cBytesReceived, cBytesSent);
#else
		sprintf(buffer, "Network usage:\nPing: %1.0fmsTotal received: %1.2f Kb\nTotal sent: %1.2f Kb\nCurrent received: %1.2f Kb\nCurrent sent: %1.2f Kb", ping, tBytesReceived, tBytesSent, cBytesReceived, cBytesSent);
#endif

		//std::string networkData = "Network usage:\nTotal received: " + std::to_string(tBytesReceived) + " Kb\nTotal sent: " + std::to_string(tBytesSent) + " Kb\nCurrent received: " + std::to_string(cBytesReceived) + " Kb\nCurrent sent: " + std::to_string(cBytesSent) + " Kb";
		RENDERER.RenderSimpleText(buffer, 0, 2);

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
				Network::PacketHandler* ph = server->GetPacketHandler();
				uint64_t id = ph->StartPack("CubePos");
				ph->WriteFloat(id, mat[100][3][0]);
				ph->WriteFloat(id, mat[100][3][1]);
				ph->WriteFloat(id, mat[100][3][2]);
				Network::Packet* p = ph->EndPack(id);
				server->Broadcast(p);
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
	SAFE_DELETE(server);
	SAFE_DELETE(client);
}

int main(int argc, char** argv)
{
	
	Start();
	return 0;
}