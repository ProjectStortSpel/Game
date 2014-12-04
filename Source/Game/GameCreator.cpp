#include "GameCreator.h"
#include "Timer.h"
#include "Systems/MovementSystem.h"
#include "Systems/RenderSystem.h"

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
}

void GameCreator::InitializeInput()
{
	m_input = &Input::InputWrapper::GetInstance();
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
	worldCreator.AddLuaSystemToCurrentGroup(new RenderSystem(m_graphics));
	m_world = worldCreator.CreateWorld(10000);
	LuaEmbedder::AddObject<ECSL::World>("World", m_world, "world");
}

void SpawnShit(ECSL::World* _world, Renderer::GraphicDevice* _graphics, bool isTrue = true)
{
	for (int x = -4; x <= 4; ++x)
	{
		for (int y = -4; y <= 4; ++y)
		{
			unsigned int newEntity = _world->CreateNewEntity();
			_world->CreateComponentAndAddTo("Position", newEntity);
			_world->CreateComponentAndAddTo("Scale", newEntity);
			_world->CreateComponentAndAddTo("Rotation", newEntity);
			_world->CreateComponentAndAddTo("Render", newEntity);

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

}

void GameCreator::StartGame()
{
	SpawnShit(m_world, m_graphics);

	Timer gameTimer;
	while (true)
	{
		float dt = gameTimer.ElapsedTimeInSeconds();
		gameTimer.Reset(); 



		m_graphics->Update(dt);
		m_input->Update();
		m_world->Update(dt);

		std::stringstream sstm;
		sstm << m_world->GetActiveEntities() << " entities";
		m_graphics->RenderSimpleText(sstm.str(), 0, 2);

		PollSDLEvent();

		if (m_input->GetKeyboard()->GetKeyState(SDL_SCANCODE_W) == Input::InputState::DOWN)
			m_graphics->GetCamera()->MoveForward(dt);
		if (m_input->GetKeyboard()->GetKeyState(SDL_SCANCODE_S) == Input::InputState::DOWN)
			m_graphics->GetCamera()->MoveBackward(dt);
		if (m_input->GetKeyboard()->GetKeyState(SDL_SCANCODE_A) == Input::InputState::DOWN)
			m_graphics->GetCamera()->MoveLeft(dt);
		if (m_input->GetKeyboard()->GetKeyState(SDL_SCANCODE_D) == Input::InputState::DOWN)
			m_graphics->GetCamera()->MoveRight(dt);

		// ROTATE CAMERA
		if (m_input->GetMouse()->GetButtonState(Input::LeftButton) == Input::InputState::DOWN)
		{
			int sizeX, sizeY;
			m_graphics->GetWindowSize(sizeX, sizeY);

			m_graphics->GetCamera()->UpdateMouse(sizeX*0.5, sizeY*0.5, m_input->GetMouse()->GetX(), m_input->GetMouse()->GetY());
			m_input->GetMouse()->SetPosition(sizeX*0.5, sizeY*0.5);
			m_input->GetMouse()->HideCursor(true);
		}
		else
			m_input->GetMouse()->HideCursor(false);

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