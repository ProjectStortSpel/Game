#include "GameCreator.h"
#include "Timer.h"
#include "Systems/MovementSystem.h"
#include "Systems/RenderSystem.h"
#include "Systems/CameraSystem.h"

GameCreator::GameCreator() :
m_graphics(0), m_input(0), m_world(0)
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

	LuaEmbedder::Quit();

	delete(&ECSL::ComponentTypeManager::GetInstance());
}

void GameCreator::InitializeGraphics()
{
	m_graphics = new Renderer::GraphicDevice();
	m_graphics->Init();
	LuaEmbedder::AddObject<Renderer::GraphicDevice>("GraphicDevice", m_graphics, "graphics");
}

void GameCreator::InitializeInput()
{
	m_input = &Input::InputWrapper::GetInstance();
}

void GameCreator::InitializeNetwork()
{

}

void GameCreator::InitializeLua()
{
	LuaEmbedder::Init();
	LuaBridge::Embed();
}

void GameCreator::InitializeWorld()
{
	//ECSL::ComponentTypeManager::GetInstance().LoadComponentTypesFromDirectory("content/components");
	ECSL::WorldCreator worldCreator = ECSL::WorldCreator();
	LuaEmbedder::AddObject<ECSL::WorldCreator>("WorldCreator", &worldCreator, "worldCreator");

	LuaEmbedder::Load("../../../Externals/content/scripting/storaspel/init.lua");

	auto componentTypes = ECSL::ComponentTypeManager::GetInstance().GetComponentTypes();
	for (auto it = componentTypes->begin(); it != componentTypes->end(); ++it)
	{
		worldCreator.AddComponentType(it->second->GetName());
		printf("%s added\n", it->second->GetName().c_str());
	}

	worldCreator.AddSystemGroup();
	worldCreator.AddSystemToCurrentGroup<MovementSystem>();
	worldCreator.AddLuaSystemToCurrentGroup(new CameraSystem(m_graphics));
	worldCreator.AddLuaSystemToCurrentGroup(new RenderSystem(m_graphics));
	m_world = worldCreator.CreateWorld(10000);
	LuaEmbedder::AddObject<ECSL::World>("World", m_world, "world");
}
unsigned int LOL;
void SpawnShit(ECSL::World* _world, Renderer::GraphicDevice* _graphics, bool isTrue = true)
{
	int size = 3;
	for (int x = -size; x <= size; ++x)
	{
		for (int y = -size; y <= size; ++y)
		{
			unsigned int newEntity = _world->CreateNewEntity();
			_world->CreateComponentAndAddTo("Position", newEntity);
			_world->CreateComponentAndAddTo("Scale", newEntity);
			_world->CreateComponentAndAddTo("Rotation", newEntity);
			_world->CreateComponentAndAddTo("Render", newEntity);
			_world->CreateComponentAndAddTo("Velocity", newEntity);

			glm::mat4*	Matrix;
			Matrix = (glm::mat4*)_world->GetComponent(newEntity, "Render", "Mat");
			int*		ModelId = (int*)_world->GetComponent(newEntity, "Render", "ModelId");

			if (y % 2 == 0 && x % 2 == 0)
				*ModelId = _graphics->LoadModel("content/models/Hole/", "hole.object", Matrix);
			else
				*ModelId = _graphics->LoadModel("content/models/Default_Tile/", "grass.object", Matrix);

			float* Position;
			Position = (float*)_world->GetComponent(newEntity, "Position", "X");
			Position[0] = 1.0f * x;
			Position[1] = 0.0f;
			Position[2] = 1.0f * y;
		}
	}

	unsigned int newEntity = _world->CreateNewEntity();
	_world->CreateComponentAndAddTo("Position", newEntity);
	_world->CreateComponentAndAddTo("Scale", newEntity);
	_world->CreateComponentAndAddTo("Rotation", newEntity);
	_world->CreateComponentAndAddTo("Render", newEntity);

	glm::mat4*	Matrix;
	Matrix = (glm::mat4*)_world->GetComponent(newEntity, "Render", "Mat");
	int* ModelId = (int*)_world->GetComponent(newEntity, "Render", "ModelId");
	*ModelId = _graphics->LoadModel("content/models/Head/", "head.object", Matrix);
	float* Position;
	Position = (float*)_world->GetComponent(newEntity, "Position", "X");
	Position[0] = 0.0f;
	Position[1] = 1.5f;
	Position[2] = 0.0f;

	LOL = newEntity;

}

void GameCreator::StartGame()
{
	SpawnShit(m_world, m_graphics);

	Timer gameTimer;
	while (true)
	{
		float dt = gameTimer.ElapsedTimeInSeconds();
		gameTimer.Reset(); 

		/*	Collect all input	*/
		m_input->Update();
		PollSDLEvent();

		/*	Update graphics	*/
		m_graphics->Update(dt);
		
		/*	Update world (systems, entities etc)	*/
		m_world->Update(dt);

		if (m_input->GetInstance().GetKeyboard()->GetKeyState(SDL_SCANCODE_O) == Input::InputState::PRESSED)
		{
			m_world->KillEntity(LOL);
		}
		if (m_input->GetInstance().GetKeyboard()->GetKeyState(SDL_SCANCODE_P) == Input::InputState::PRESSED)
		{
			unsigned int newEntity = m_world->CreateNewEntity();
			m_world->CreateComponentAndAddTo("Position", newEntity);
			m_world->CreateComponentAndAddTo("Scale", newEntity);
			m_world->CreateComponentAndAddTo("Rotation", newEntity);
			m_world->CreateComponentAndAddTo("Render", newEntity);

			glm::mat4*	Matrix;
			Matrix = (glm::mat4*)m_world->GetComponent(newEntity, "Render", "Mat");
			int* ModelId = (int*)m_world->GetComponent(newEntity, "Render", "ModelId");
			*ModelId = m_graphics->LoadModel("content/models/Head/", "head.object", Matrix);
			float* Position;
			Position = (float*)m_world->GetComponent(newEntity, "Position", "X");
			Position[0] = 0.0f;
			Position[1] = 1.5f;
			Position[2] = 0.0f;

			LOL = newEntity;
		}

		std::stringstream sstm;
		sstm << m_world->GetActiveEntities() << " entities";
		m_graphics->RenderSimpleText(sstm.str(), 0, 2);



		if (m_input->GetKeyboard()->GetKeyState(SDL_SCANCODE_ESCAPE) == Input::InputState::PRESSED)
			break;

		m_graphics->Render();
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