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
	/*	Fulhax just nu, enbart för att slippa ha det i gameloopen (REN OCH FIN)	*/

	if (m_input->GetKeyboard()->GetKeyState(SDL_SCANCODE_W) == Input::InputState::DOWN)
		m_graphics->GetCamera()->MoveForward(_dt);
	if (m_input->GetKeyboard()->GetKeyState(SDL_SCANCODE_S) == Input::InputState::DOWN)
		m_graphics->GetCamera()->MoveBackward(_dt);
	if (m_input->GetKeyboard()->GetKeyState(SDL_SCANCODE_A) == Input::InputState::DOWN)
		m_graphics->GetCamera()->MoveLeft(_dt);
	if (m_input->GetKeyboard()->GetKeyState(SDL_SCANCODE_D) == Input::InputState::DOWN)
		m_graphics->GetCamera()->MoveRight(_dt);

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

}

void CameraSystem::OnEntityAdded(unsigned int _entityId)
{

}

void CameraSystem::OnEntityRemoved(unsigned int _entityId)
{

}