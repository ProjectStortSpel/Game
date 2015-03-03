#include "GameConsole.h"
#include "ECSL/Managers/EntityTemplateManager.h"
#include <string>
#include "LuaEmbedder/LuaEmbedder.h"
#include "Network/ClientDatabase.h"

GameConsole::GameConsole(Renderer::GraphicDevice* _graphics, ECSL::World* _world)
{
	m_graphics = _graphics;
	m_world = _world;
}

GameConsole::~GameConsole()
{
}

void GameConsole::CreateObject(std::string _command, std::vector<Console::Argument>* _args)
{
	if (_args->at(0).ArgType != Console::ArgumentType::Text)
		return;

	std::string _template = _args->at(0).Text;
	_template[0] = toupper(_template[0]);
	if (!ECSL::EntityTemplateManager::GetInstance().HasTemplate(_template))
	{
		std::stringstream ss;
		ss << "Template \"" << _template << "\" is not valid!";
		m_consoleManager->AddMessage(ss.str().c_str());
		return;
	}
		

	unsigned int mId = m_world->CreateNewEntity(_template);
	m_world->CreateComponentAndAddTo("ChangedComponents", mId);

	if (!m_world->HasComponent(mId, "SyncNetwork"))
		m_world->CreateComponentAndAddTo("SyncNetwork", mId);

	std::stringstream ss;
	ss << "Entity with id #" << mId << " has been created!";
	m_consoleManager->AddMessage(ss.str().c_str());

	if (_args->size() > 1)
	{
		float* objectPosition;
		objectPosition = (float*)m_world->GetComponent(mId, "Position", 0);
		objectPosition[0] = _args->at(1).Number;
		objectPosition[1] = _args->at(2).Number;
		objectPosition[2] = _args->at(3).Number;
	}
}

void GameConsole::RemoveObject(std::string _command, std::vector<Console::Argument>* _args)
{
	if (_args->size() > 0 && (*_args)[0].ArgType == Console::ArgumentType::Number)
	{
		std::stringstream ss;
		ss << "Entity #" << (*_args)[0].Number << " removed!";
		m_consoleManager->AddMessage(ss.str().c_str());

		m_world->KillEntity((*_args)[0].Number);

		/*if (NetworkInstance::GetServer()->IsRunning())
		{
			Network::PacketHandler* ph = NetworkInstance::GetServer()->GetPacketHandler();
			NetworkInstance::GetServer()->Broadcast(NetworkInstance::GetNetworkHelper()->WriteEntityKill(ph, (*_args)[0].Number));
		}*/
	}
}

void GameConsole::AddComponent(std::string _command, std::vector<Console::Argument>* _args)
{
	/*	Is the command valid in syntax?	*/
	if (_args->size() < 2)
		return;
	if (_args->at(0).ArgType != Console::ArgumentType::Number)
		return;
	if (_args->at(1).ArgType != Console::ArgumentType::Text)
		return;

	/*	Get component type	*/
	std::string componentType = _args->at(1).Text;
	componentType[0] = toupper(componentType[0]);

	if (!ECSL::ComponentTypeManager::GetInstance().ComponentExists(componentType))
	{
		std::stringstream ss;
		ss << "Component \"" <<_args->at(1).Text << "\" was not found!";
		m_consoleManager->AddMessage(ss.str().c_str());
		return;
	}
		

	unsigned int mId = (unsigned int)_args->at(0).Number;
	m_world->CreateComponentAndAddTo(componentType, mId);

	std::stringstream ss;
	ss << "Entity #" << mId << " now has " << _args->at(1).Text;
	m_consoleManager->AddMessage(ss.str().c_str());

	if (_args->size() > 2)
		ChangeComponent(_command, _args);
}

void GameConsole::ChangeComponent(std::string _command, std::vector<Console::Argument>* _args)
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

	unsigned int componentTypeId = ECSL::ComponentTypeManager::GetInstance().GetTableId(componentType);
	int bitSetIndex = ECSL::BitSet::GetBitSetIndex(componentTypeId);
	int bitIndex = ECSL::BitSet::GetBitIndex(componentTypeId);

	ECSL::BitSet::DataType* changedComponents = (ECSL::BitSet::DataType*)m_world->GetComponent(mId, "ChangedComponents", 0);
	changedComponents[bitSetIndex] |= ((ECSL::BitSet::DataType)1) << bitIndex;

	ECSL::BitSet::DataType* changedComponentsNetwork = (ECSL::BitSet::DataType*)m_world->GetComponent(mId, "ChangedComponentsNetwork", 0);
	changedComponentsNetwork[bitSetIndex] |= ((ECSL::BitSet::DataType)1) << bitIndex;
}

void GameConsole::RemoveComponent(std::string _command, std::vector<Console::Argument>* _args)
{
	unsigned int mId = (unsigned int)_args->at(0).Number;

	std::string componentType = _args->at(1).Text;
	componentType[0] = toupper(componentType[0]);
	m_world->RemoveComponentFrom(componentType, mId);
}

void GameConsole::ClearHistory(std::string _command, std::vector<Console::Argument>* _args)
{
	m_consoleManager->ClearHistory();
}

void GameConsole::HostServer(std::string _command, std::vector<Console::Argument>* _args)
{
	if (NetworkInstance::GetClient()->IsConnected())
	{
		NetworkInstance::GetClient()->Disconnect();
		//LuaEmbedder::AddBool("Client", false);
	}
	if (NetworkInstance::GetServer()->IsRunning())
	{
		NetworkInstance::GetServer()->Stop();
		//LuaEmbedder::AddBool("Server", false);
	}


	std::string pw				= NetworkInstance::GetServer()->GetServerPassword();
	unsigned int port			= NetworkInstance::GetServer()->GetIncomingPort();
	unsigned int connections	= NetworkInstance::GetServer()->GetMaxConnections();

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

	bool hosting = NetworkInstance::GetServer()->Start(port, pw.c_str(), connections);

	//LuaEmbedder::AddBool("Server", hosting);
	//NetworkInstance::GetClient()->Connect("127.0.0.1", pw.c_str(), port, 0);
}

void GameConsole::HostListenServer(std::string _command, std::vector<Console::Argument>* _args)
{
    if (NetworkInstance::GetClient()->IsConnected())
    {
        NetworkInstance::GetClient()->Disconnect();
        //LuaEmbedder::AddBool("Client", false);
    }
    if (NetworkInstance::GetServer()->IsRunning())
    {
        NetworkInstance::GetServer()->Stop();
        //LuaEmbedder::AddBool("Server", false);
    }
    
    
    std::string pw				= NetworkInstance::GetServer()->GetServerPassword();
    unsigned int port			= NetworkInstance::GetServer()->GetIncomingPort();
    unsigned int connections	= NetworkInstance::GetServer()->GetMaxConnections();
    
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
    
    bool hosting = NetworkInstance::GetServer()->Start(port, pw.c_str(), connections);
    
    //LuaEmbedder::AddBool("Server", hosting);
    
    bool connected = NetworkInstance::GetClient()->Connect("127.0.0.1", pw.c_str(), port, 0);
}



void GameConsole::StopServer(std::string _command, std::vector<Console::Argument>* _args)
{
	if (NetworkInstance::GetClient()->IsConnected())
	{
		NetworkInstance::GetClient()->Disconnect();
	}

	if (NetworkInstance::GetServer()->IsRunning())
	{
		NetworkInstance::GetServer()->Stop();
		//LuaEmbedder::AddBool("Server", false);
	}
}

void GameConsole::ConnectClient(std::string _command, std::vector<Console::Argument>* _args)
{
	if (NetworkInstance::GetClient()->IsConnected())
	{
		NetworkInstance::GetClient()->Disconnect();
		//LuaEmbedder::AddBool("Client", false);
	}

	
	if (NetworkInstance::GetServer()->IsRunning())
	{
		NetworkInstance::GetServer()->Stop();
		//LuaEmbedder::AddBool("Server", false);
	}
	

	std::string ip		= NetworkInstance::GetClient()->GetRemoteAddress();
	unsigned int port	= NetworkInstance::GetClient()->GetOutgoingPort();
	std::string pw		= NetworkInstance::GetClient()->GetServerPassword();
	

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

	bool connected = NetworkInstance::GetClient()->Connect(ip.c_str(), pw.c_str(), port, 0);
	//LuaEmbedder::AddBool("Client", connected);
}

void GameConsole::BroadcastChat(std::string _command, std::vector<Console::Argument>* _args)
{
	if (!NetworkInstance::GetClient()->IsConnected())
		return;

	std::stringstream ss;
	for (int i = 0; i < _args->size(); ++i)
	{
		if (i > 0)
			ss << " ";

		if (_args->at(i).ArgType == Console::ArgumentType::Text)
		{
			ss << _args->at(i).Text;
		}
		else
		{
			ss << _args->at(i).Number;
		}
	}

	std::string temp = ss.str();
	auto ph = NetworkInstance::GetClient()->GetPacketHandler();
	auto id = ph->StartPack("LuaPacket");
	ph->WriteString(id, "CLIENT_SEND_CHAT_MESSAGE");
	ph->WriteString(id, temp.c_str());
	NetworkInstance::GetClient()->Send(ph->EndPack(id));

//LuaEmbedder::AddBool("Client", connected);
}

void GameConsole::BroadcastAnnouncement(std::string _command, std::vector<Console::Argument>* _args)
{
	if (!NetworkInstance::GetClient()->IsConnected())
		return;

	std::string temp = _args->at(0).GetString();
	SDL_Log("%s", temp.c_str());
		//LuaEmbedder::AddBool("Client", connected);
}


void GameConsole::DisconnectClient(std::string _command, std::vector<Console::Argument>* _args)
{
	if (NetworkInstance::GetClient()->IsConnected())
	{
		NetworkInstance::GetClient()->Disconnect();
		//LuaEmbedder::AddBool("Client", false);
	}
}

void GameConsole::SetDebugTexture(std::string _command, std::vector<Console::Argument>* _args)
{
	if (_args->size() == 0)
		return;

	if ((*_args)[0].ArgType == Console::ArgumentType::Text)
	{
		if (strcmp((*_args)[0].Text, "standard") == 0)
			m_graphics->SetDebugTexFlag(0);

		else if (strcmp((*_args)[0].Text, "diffuse") == 0)
			m_graphics->SetDebugTexFlag(1);

		else if (strcmp((*_args)[0].Text, "normal") == 0)
			m_graphics->SetDebugTexFlag(2);

		else if (strcmp((*_args)[0].Text, "specular") == 0)
			m_graphics->SetDebugTexFlag(3);

		else if (strcmp((*_args)[0].Text, "glow") == 0)
			m_graphics->SetDebugTexFlag(4);
	}
}

void GameConsole::ToggleText(std::string _command, std::vector<Console::Argument>* _args)
{
	if (_args->size() == 0)
	{
		m_graphics->ToggleSimpleText();
		return;
	}

	if (strcmp((*_args)[0].Text, "ON") == 0)
		m_graphics->ToggleSimpleText(true);

	else if (strcmp((*_args)[0].Text, "OFF") == 0)
		m_graphics->ToggleSimpleText(false);
}

void GameConsole::SetTextColor(std::string _command, std::vector<Console::Argument>* _args)
{
	if (_args->size() == 0)
		return;

	if ((*_args)[0].ArgType == Console::ArgumentType::Text)
		if (strcmp((*_args)[0].Text, "DISCO") == 0)
			m_graphics->SetDisco();

	if (_args->size() != 3)
		return;

	if ((*_args)[0].ArgType == Console::ArgumentType::Number &&
		(*_args)[1].ArgType == Console::ArgumentType::Number &&
		(*_args)[2].ArgType == Console::ArgumentType::Number)
	{
		float r, g, b;
		r = _args->at(0).Number;
		g = _args->at(1).Number;
		b = _args->at(2).Number;
		m_graphics->SetSimpleTextColor(r, g, b, 1);
	}
}

void GameConsole::AddPointlight(std::string _command, std::vector<Console::Argument>* _args)
{
	if (_args->size() < 4)
		return;

	for (int n = 0; n < _args->size(); ++n)
		if (_args->at(n).ArgType != Console::ArgumentType::Number)
			return;

	unsigned int newLight = m_world->CreateNewEntity();
	m_world->CreateComponentAndAddTo("Pointlight", newLight);
	float* lightData = (float*)m_world->GetComponent(newLight, "Pointlight", 0);
	//	Position
	lightData[0] = _args->at(0).Number;
	lightData[1] = _args->at(1).Number;
	lightData[2] = _args->at(2).Number;
	//	Intensity
	lightData[3] = 0.5f;
	lightData[4] = 1.0f;
	lightData[5] = 1.0f;
	//	Color
	lightData[6] = 1.0f;
	lightData[7] = 0.8f;
	lightData[8] = 0.8f;
	//	Range
	lightData[9] = _args->at(3).Number;

	std::stringstream ss;
	ss << "Pointlight with id #" << newLight << " has been created!";
	m_consoleManager->AddMessage(ss.str().c_str());
}

void GameConsole::ECSLMemoryUsage(std::string _command, std::vector<Console::Argument>* _args)
{
	std::stringstream ss;
	ss << m_world->GetMemoryUsage() << " mb is allocated by ECSL!";
	m_consoleManager->AddMessage(ss.str().c_str());
}

void GameConsole::ListCommands(std::string _command, std::vector<Console::Argument>* _args)
{
	m_consoleManager->AddMessage("Command           -   Arg1, Arg2, Arg3, ...");
	m_consoleManager->AddMessage("CreateObject      -   Model, Path, X, Y, Z");
	m_consoleManager->AddMessage("RemoveObject      -   Id");
	m_consoleManager->AddMessage("AddComponent      -   Id, ComponentType");
	m_consoleManager->AddMessage("ChangeComponent   -   Id, ComponentType, X, Y, Z, ...");
	m_consoleManager->AddMessage("RemoveComponent   -   Id, ComponentType");
	m_consoleManager->AddMessage("Host              -   Port, Password, MaxConnections");
    m_consoleManager->AddMessage("HostListen        -   Port, Password, MaxConnections");
	m_consoleManager->AddMessage("Connect           -   Ip-address, Port, Password");
	m_consoleManager->AddMessage("ToggleText        -   ON/OFF");
	m_consoleManager->AddMessage("TextColor         -   R G B");
	m_consoleManager->AddMessage("DebugRender       -   RenderType");
	m_consoleManager->AddMessage("Disconnect");
	m_consoleManager->AddMessage("Stop");
}

void GameConsole::SetupHooks(Console::ConsoleManager* _consoleManager)
{
	m_consoleManager = _consoleManager;
	m_consoleManager->AddCommand("CreateObject", std::bind(&GameConsole::CreateObject, this, std::placeholders::_1, std::placeholders::_2));
	m_consoleManager->AddCommand("RemoveObject", std::bind(&GameConsole::RemoveObject, this, std::placeholders::_1, std::placeholders::_2));
	m_consoleManager->AddCommand("AddComponent", std::bind(&GameConsole::AddComponent, this, std::placeholders::_1, std::placeholders::_2));
	m_consoleManager->AddCommand("ChangeComponent", std::bind(&GameConsole::ChangeComponent, this, std::placeholders::_1, std::placeholders::_2));
	m_consoleManager->AddCommand("RemoveComponent", std::bind(&GameConsole::RemoveComponent, this, std::placeholders::_1, std::placeholders::_2));
	m_consoleManager->AddCommand("Host", std::bind(&GameConsole::HostServer, this, std::placeholders::_1, std::placeholders::_2));
    m_consoleManager->AddCommand("HostListen", std::bind(&GameConsole::HostListenServer, this, std::placeholders::_1, std::placeholders::_2));
	m_consoleManager->AddCommand("Stop", std::bind(&GameConsole::StopServer, this, std::placeholders::_1, std::placeholders::_2));
	m_consoleManager->AddCommand("Connect", std::bind(&GameConsole::ConnectClient, this, std::placeholders::_1, std::placeholders::_2));
	m_consoleManager->AddCommand("Disconnect", std::bind(&GameConsole::DisconnectClient, this, std::placeholders::_1, std::placeholders::_2));
	m_consoleManager->AddCommand("List", std::bind(&GameConsole::ListCommands, this, std::placeholders::_1, std::placeholders::_2));
	m_consoleManager->AddCommand("Clear", std::bind(&GameConsole::ClearHistory, this, std::placeholders::_1, std::placeholders::_2));

	m_consoleManager->AddCommand("Say", std::bind(&GameConsole::BroadcastChat, this, std::placeholders::_1, std::placeholders::_2));
	m_consoleManager->AddCommand("Announce", std::bind(&GameConsole::BroadcastAnnouncement, this, std::placeholders::_1, std::placeholders::_2));

	m_consoleManager->AddCommand("ToggleText", std::bind(&GameConsole::ToggleText, this, std::placeholders::_1, std::placeholders::_2));
	m_consoleManager->AddCommand("TextColor", std::bind(&GameConsole::SetTextColor, this, std::placeholders::_1, std::placeholders::_2));

	m_consoleManager->AddCommand("DebugRender", std::bind(&GameConsole::SetDebugTexture, this, std::placeholders::_1, std::placeholders::_2));
	m_consoleManager->AddCommand("AddPointlight", std::bind(&GameConsole::AddPointlight, this, std::placeholders::_1, std::placeholders::_2));

	m_consoleManager->AddCommand("MemoryUsage", std::bind(&GameConsole::ECSLMemoryUsage, this, std::placeholders::_1, std::placeholders::_2));
}

