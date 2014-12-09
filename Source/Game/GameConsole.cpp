#include "GameConsole.h"
#include <string>
GameConsole::GameConsole(Renderer::GraphicDevice* _graphics, ECSL::World* _world)
{
	m_graphics = _graphics;
	m_world = _world;
}

GameConsole::~GameConsole()
{

}



void GameConsole::SpawnModel(std::vector<Console::Argument>* _args)
{
	if (_args->size() != 5)
		return;

	std::string _modelName = _args->at(0).Text;
	_modelName.append(".object");
	std::string _modelPath = "content/models/";
	_modelPath.append(_args->at(1).Text);

	float x = _args->at(2).Number;
	float y = _args->at(3).Number;
	float z = _args->at(4).Number;

	unsigned int mId = m_world->CreateNewEntity();
	m_world->CreateComponentAndAddTo("Position", mId);
	m_world->CreateComponentAndAddTo("Scale", mId);
	m_world->CreateComponentAndAddTo("Rotation", mId);
	m_world->CreateComponentAndAddTo("Render", mId);

	glm::mat4*	Matrix;
	Matrix = (glm::mat4*)m_world->GetComponent(mId, "Render", "Mat");
	int* ModelId = (int*)m_world->GetComponent(mId, "Render", "ModelId");
	*ModelId = m_graphics->LoadModel(_modelPath, _modelName, Matrix);

	float* Position;
	Position = (float*)m_world->GetComponent(mId, "Position", "X");
	Position[0] = x;
	Position[1] = y;
	Position[2] = z;

	std::stringstream ss;
	ss << "Entity with id #" << mId << " has been created!";
	m_consoleManager->AddMessage(ss.str().c_str());
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
	m_consoleManager->AddMessage("LoadModel - Model, Path, X, Y, Z");
	m_consoleManager->AddMessage("AddComponent - Id, ComponentType");
	m_consoleManager->AddMessage("ChangeComponent - Id, ComponentType, X, Y, Z, ...");
	m_consoleManager->AddMessage("RemoveComponent - Id, ComponentType");
}

void GameConsole::SetupHooks(Console::ConsoleManager* _consoleManager)
{
	m_consoleManager = _consoleManager;
	m_consoleManager->AddCommand("LoadModel", std::bind(&GameConsole::SpawnModel, this, std::placeholders::_1));
	m_consoleManager->AddCommand("AddComponent", std::bind(&GameConsole::AddComponent, this, std::placeholders::_1));
	m_consoleManager->AddCommand("ChangeComponent", std::bind(&GameConsole::ChangeComponent, this, std::placeholders::_1));
	m_consoleManager->AddCommand("RemoveComponent", std::bind(&GameConsole::RemoveComponent, this, std::placeholders::_1));
	m_consoleManager->AddCommand("List", std::bind(&GameConsole::ListCommands, this, std::placeholders::_1));
}