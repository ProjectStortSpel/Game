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



void GameConsole::SpawnGraphicalObjectAt(std::vector<Console::Argument>* _args)
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


void GameConsole::SetupHooks(Console::ConsoleManager* _consoleManager)
{
	m_consoleManager = _consoleManager;
	m_consoleManager->AddCommand("SpawnModel", std::bind(&GameConsole::SpawnGraphicalObjectAt, this, std::placeholders::_1));
}