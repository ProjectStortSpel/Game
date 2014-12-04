#include "GameCreator.h"
#include "Timer.h"



class LOLSYSTEM : public ECSL::System
{
public:
	LOLSYSTEM(Renderer::GraphicDevice* _graphics) { m_graphics = _graphics; lol = false; }
	~LOLSYSTEM() { }

	void Update(float _dt)
	{


		auto entites = *GetEntities();
		for (auto entity : entites)
		{
			float *Xp, *Yp, *Zp;
			Xp = (float*)GetComponent((unsigned int)entity, 0, 0);
			Yp = (float*)GetComponent(entity, "Position", "Y");
			Zp = (float*)GetComponent(entity, "Position", "Z");

			*Xp += 0.6f * _dt;

			glm::mat4* eMat = (glm::mat4*)GetComponent(entity, "Render", "Mat");
			*eMat = glm::translate(glm::vec3(*Xp, *Yp, *Zp));

		}

		//m_graphics->RenderSimpleText("")
	}
	void Initialize()
	{
		AddComponentTypeToFilter("Position", ECSL::FilterType::Mandatory);
		AddComponentTypeToFilter("Velocity", ECSL::FilterType::Mandatory);
		AddComponentTypeToFilter("Render", ECSL::FilterType::Mandatory);
		//AddComponentTypeToFilter("Velocity", ECSL::ComponentFilter::RequiresOneOf);
		//AddComponentTypeToFilter("Position", ECSL::ComponentFilter::Excluded);

		printf("Testsystem Initialize()\n");
	}

	void OnEntityAdded(unsigned int _entityId)
	{
		printf("Testsystem OnLOLSYSTEMLOLSYSTEMLOLSYSTEMLOLSYSTEMEntityAdded()\n");
		int* eModelId = (int*)GetComponent(_entityId, "Render", "ModelId");
		glm::mat4* eMat = (glm::mat4*)GetComponent(_entityId, "Render", "Mat");
		*eMat = glm::translate(glm::vec3(1));

		if (!lol)
		{
			*eModelId = m_graphics->LoadModel("content/models/cube/", "cube.object", eMat);
			int b = *eModelId;
			int a = 2;
		}
		else
		{
			*eModelId = m_graphics->LoadModel("content/models/cube2/", "cube2.object", eMat);
			int b = *eModelId;
			int a = 2;
		}
		lol = true;
	}
	void OnEntityRemoved(unsigned int _entityId)
	{
		printf("Testsystem OnEntityRemoved()\n");

		/*	PLocka ut modelid -> säga till grafiken att det idet är inaktivt	*/
	}
private:
	Renderer::GraphicDevice* m_graphics;


	bool lol;
};


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
	worldCreator.AddLuaSystemToCurrentGroup(new LOLSYSTEM(m_graphics));
	m_world = worldCreator.CreateWorld(100);
	LuaEmbedder::AddObject<ECSL::World>("World", m_world, "world");
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


void GameCreator::StartGame()
{
	Timer gameTimer;
	while (true)
	{
		float dt = gameTimer.ElapsedTimeInSeconds();
		gameTimer.Reset(); 



		m_graphics->Update(dt);
		m_input->Update();
		m_world->Update(dt);

		//m_graphics->RenderSimpleText("lol", 0, 2);

		PollSDLEvent();

		if (m_input->GetKeyboard()->GetKeyState(SDL_SCANCODE_SPACE) == Input::InputState::PRESSED)
		{
			unsigned int newEntity = m_world->CreateNewEntity();
			m_world->CreateComponentAndAddTo("Position", newEntity);
			//m_world->CreateComponentAndAddTo("Velocity", newEntity);
			//m_world->CreateComponentAndAddTo("Render", newEntity);

		}
		if (m_input->GetKeyboard()->GetKeyState(SDL_SCANCODE_H) == Input::InputState::PRESSED)
		{
			m_world->KillEntity(0);
		}

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