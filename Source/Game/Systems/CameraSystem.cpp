#include "CameraSystem.h"

CameraSystem::CameraSystem(Renderer::GraphicDevice* _graphics)
{
	m_graphics = _graphics;
}
CameraSystem::~CameraSystem()
{

}

void CameraSystem::Initialize()
{
	/*	CameraSystem wants a position to place the camera at	*/
	AddComponentTypeToFilter("Camera", ECSL::FilterType::Mandatory);

	m_input = &Input::InputWrapper::GetInstance();
	printf("CameraSystem initialized!\n");
}

void CameraSystem::Update(float _dt)
{
	if (GetEntities()->size() == 0)
		return;

	if (m_input->GetKeyboard()->GetKeyState(SDL_SCANCODE_W) == Input::InputState::DOWN)
		m_graphics->GetCamera()->MoveForward(_dt);
	if (m_input->GetKeyboard()->GetKeyState(SDL_SCANCODE_S) == Input::InputState::DOWN)
		m_graphics->GetCamera()->MoveBackward(_dt);
	if (m_input->GetKeyboard()->GetKeyState(SDL_SCANCODE_A) == Input::InputState::DOWN)
		m_graphics->GetCamera()->MoveLeft(_dt);
	if (m_input->GetKeyboard()->GetKeyState(SDL_SCANCODE_D) == Input::InputState::DOWN)
		m_graphics->GetCamera()->MoveRight(_dt);

}

void CameraSystem::OnEntityAdded(unsigned int _entityId)
{

}

void CameraSystem::OnEntityRemoved(unsigned int _entityId)
{

}