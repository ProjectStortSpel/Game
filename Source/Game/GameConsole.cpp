#include "GameConsole.h"
#include <string>
GameConsole::GameConsole(Renderer::GraphicDevice* _graphics, ECSL::World* _world, Network::ClientNetwork* _client, Network::ServerNetwork* _server)
{
	m_graphics = _graphics;
	m_world = _world;
	m_client = _client;
	m_server = _server;
	m_networkHelper = new NetworkHelper(_world);
}

GameConsole::~GameConsole()
{
	delete m_networkHelper;
}

void GameConsole::CreateObject(std::vector<Console::Argument>* _args)
{
	if (_args->size() != 5)
		return;

	std::string _modelName = _args->at(0).Text;
	//_modelName.append(".object");
	//std::string _modelPath = "content/models/";
	std::string _modelPath = _args->at(1).Text;

	float x = _args->at(2).Number;
	float y = _args->at(3).Number;
	float z = _args->at(4).Number;

	unsigned int mId = m_world->CreateNewEntity();
	m_world->CreateComponentAndAddTo("Position", mId);
	m_world->CreateComponentAndAddTo("Scale", mId);
	m_world->CreateComponentAndAddTo("Rotation", mId);
	m_world->CreateComponentAndAddTo("Model", mId);

	char* data;
	data = (char*)m_world->GetComponent(mId, "Model", "ModelName");
	for (int n = 0; n < _modelName.size(); ++n)
		data[n] = _modelName[n];
	data[_modelName.size()] = '\0';

	data = (char*)m_world->GetComponent(mId, "Model", "ModelPath");
	for (int n = 0; n < _modelPath.size(); ++n)
		data[n] = _modelPath[n];
	data[_modelPath.size()] = '\0';


	float* Position;
	Position = (float*)m_world->GetComponent(mId, "Position", "X");
	Position[0] = x;
	Position[1] = y;
	Position[2] = z;

	std::stringstream ss;
	ss << "Entity with id #" << mId << " has been created!";
	m_consoleManager->AddMessage(ss.str().c_str());

	if (m_server->IsRunning())
	{
		Network::Packet* p = m_networkHelper->WriteEntity(m_server->GetPacketHandler(), mId);
		m_server->Broadcast(p);
	}
}

void GameConsole::RemoveObject(std::vector<Console::Argument>* _args)
{
	if (_args->size() > 0 && (*_args)[0].ArgType == Console::ArgumentType::Number)
		m_world->KillEntity((*_args)[0].Number);
}

void GameConsole::AddComponent(std::vector<Console::Argument>* _args)
{
	unsigned int mId = (unsigned int)_args->at(0).Number;

	std::string componentType = _args->at(1).Text;
	componentType[0] = toupper(componentType[0]);

	m_world->CreateComponentAndAddTo(componentType, mId);

	std::stringstream ss;
	ss << "Entity #" << mId << " now has " << _args->at(1).Text;
	m_consoleManager->AddMessage(ss.str().c_str());

	if (_args->size() > 2)
		ChangeComponent(_args);
}

void GameConsole::ChangeComponent(std::vector<Console::Argument>* _args)
{
	if (_args->size() < 3)
		return;

	unsigned int mId = (unsigned int)_args->at(0).Number;

	std::string componentType = _args->at(1).Text;
	componentType[0] = toupper(componentType[0]);

	int nData = _args->size() - 2;

	float* DATA;
	DATA = (float*)m_world->GetComponent(mId, componentType, 0);
	for (int n = 0; n < nData; ++n)
		DATA[n] = _args->at(2 + n).Number;
}

void GameConsole::RemoveComponent(std::vector<Console::Argument>* _args)
{
	unsigned int mId = (unsigned int)_args->at(0).Number;

	std::string componentType = _args->at(1).Text;
	componentType[0] = toupper(componentType[0]);
	m_world->RemoveComponentFrom(componentType, mId);
}

void GameConsole::ListCommands(std::vector<Console::Argument>* _args)
{
	m_consoleManager->AddMessage("CreateObject - Model, Path, X, Y, Z");
	m_consoleManager->AddMessage("RemoveObject - Id");
	m_consoleManager->AddMessage("AddComponent - Id, ComponentType");
	m_consoleManager->AddMessage("ChangeComponent - Id, ComponentType, X, Y, Z, ...");
	m_consoleManager->AddMessage("RemoveComponent - Id, ComponentType");
	m_consoleManager->AddMessage("Host - Port, Password, MaxConnections");
	m_consoleManager->AddMessage("Stop");
	m_consoleManager->AddMessage("Connect - Ip-address, Port, Password");
	m_consoleManager->AddMessage("Disconnect");
}


void GameConsole::HostServer(std::vector<Console::Argument>* _args)
{
	if (m_client->IsConnected())
		m_client->Disconnect();

	if (m_server->IsRunning())
		m_server->Stop();


	std::string pw = m_server->GetServerPassword();
	unsigned int port = m_server->GetIncomingPort();
	unsigned int connections = m_server->GetMaxConnections();

	if (_args->size() == 1)
	{
		if ((*_args)[0].ArgType == Console::ArgumentType::Number)
			port = (*_args)[0].Number;
	}
	else if (_args->size() == 2)
	{
		if ((*_args)[0].ArgType == Console::ArgumentType::Number &&
			(*_args)[1].ArgType == Console::ArgumentType::Text)
		{
			port = (*_args)[0].Number;
			pw = (*_args)[1].Text;
		}
	}
	else if (_args->size() == 3)
	{
		if ((*_args)[0].ArgType == Console::ArgumentType::Number &&
			(*_args)[1].ArgType == Console::ArgumentType::Text &&
			(*_args)[2].ArgType == Console::ArgumentType::Number)
		{
			port		= (*_args)[0].Number;
			pw			= (*_args)[1].Text;
			connections = (*_args)[2].Number;
		}
	}

	m_server->Start(port, pw.c_str(), connections);

	//m_client->Connect("127.0.0.1", pw.c_str(), port, 0);
}
void GameConsole::StopServer(std::vector<Console::Argument>* _args)
{
	if (m_client->IsConnected() && strcmp(m_client->GetRemoteAddress(), "127.0.0.1") == 0)
		m_client->Disconnect();

	if (m_server->IsRunning())
		m_server->Stop();
}

void GameConsole::ConnectClient(std::vector<Console::Argument>* _args)
{
	if (m_client->IsConnected())
		m_client->Disconnect();

	if (m_server->IsRunning())
		m_server->Stop();

	std::string ip = m_client->GetRemoteAddress();
	unsigned int port = m_client->GetOutgoingPort();
	std::string pw = m_client->GetServerPassword();
	

	if (_args->size() == 1)
	{
		if ((*_args)[0].ArgType == Console::ArgumentType::Text)
			ip = (*_args)[0].Text;
		
	}
	else if (_args->size() == 2)
	{
		if ((*_args)[0].ArgType == Console::ArgumentType::Text &&
			(*_args)[1].ArgType == Console::ArgumentType::Number)
		{
			ip = (*_args)[0].Text;
			port = (*_args)[1].Number;
		}
	}
	else if (_args->size() == 3)
	{
		if ((*_args)[0].ArgType == Console::ArgumentType::Text &&
			(*_args)[1].ArgType == Console::ArgumentType::Number &&
			(*_args)[2].ArgType == Console::ArgumentType::Text)
		{
			ip = (*_args)[0].Text;
			port = (*_args)[1].Number;
			pw = (*_args)[2].Text;
		}
	}


	m_client->AddNetworkHook("Entity", std::bind(&NetworkHelper::ReadEntity, m_networkHelper, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
	m_client->Connect(ip.c_str(), pw.c_str(), port, 0);

}

void GameConsole::DisconnectClient(std::vector<Console::Argument>* _args)
{
	if (m_client->IsConnected())
		m_client->Disconnect();
}


void GameConsole::SetupHooks(Console::ConsoleManager* _consoleManager)
{
	m_consoleManager = _consoleManager;
	m_consoleManager->AddCommand("CreateObject", std::bind(&GameConsole::CreateObject, this, std::placeholders::_1));
	m_consoleManager->AddCommand("RemoveObject", std::bind(&GameConsole::RemoveObject, this, std::placeholders::_1));
	m_consoleManager->AddCommand("AddComponent", std::bind(&GameConsole::AddComponent, this, std::placeholders::_1));
	m_consoleManager->AddCommand("ChangeComponent", std::bind(&GameConsole::ChangeComponent, this, std::placeholders::_1));
	m_consoleManager->AddCommand("RemoveComponent", std::bind(&GameConsole::RemoveComponent, this, std::placeholders::_1));
	m_consoleManager->AddCommand("Host", std::bind(&GameConsole::HostServer, this, std::placeholders::_1));
	m_consoleManager->AddCommand("Stop", std::bind(&GameConsole::StopServer, this, std::placeholders::_1));
	m_consoleManager->AddCommand("Connect", std::bind(&GameConsole::ConnectClient, this, std::placeholders::_1));
	m_consoleManager->AddCommand("Disconnect", std::bind(&GameConsole::DisconnectClient, this, std::placeholders::_1));
	m_consoleManager->AddCommand("List", std::bind(&GameConsole::ListCommands, this, std::placeholders::_1));
}