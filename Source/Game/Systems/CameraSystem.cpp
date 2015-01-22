#include "CameraSystem.h"
#include <glm/glm.hpp>

CameraSystem::CameraSystem(Renderer::GraphicDevice* _graphics)
{
	m_graphics = _graphics;
}
CameraSystem::~CameraSystem()
{

}

void CameraSystem::Initialize()
{
	SetSystemName("Camera System");

	SetUpdateTaskCount(1);

	/*	CameraSystem wants a position to place the camera at	*/
	AddComponentTypeToFilter("Camera", ECSL::FilterType::Mandatory);

	m_input = &Input::InputWrapper::GetInstance();
	printf("CameraSystem initialized!\n");
}

void CameraSystem::Update(const ECSL::RuntimeInfo& _runtime)
{
#ifdef __ANDROID__
	static float prevDistance0, prevDistance1;
	if (m_input->GetTouch()->GetFingerState(0) == Input::InputState::PRESSED ||
		m_input->GetTouch()->GetFingerState(1) == Input::InputState::PRESSED)
	{
		glm::vec2 halfSize = glm::vec2(0.5f, 0.5f);
		prevDistance0 = glm::length(glm::vec2(m_input->GetTouch()->GetX(0), m_input->GetTouch()->GetY(0)) - halfSize);
		prevDistance1 = glm::length(glm::vec2(m_input->GetTouch()->GetX(1), m_input->GetTouch()->GetY(1)) - halfSize);
	}
	if (m_input->GetTouch()->GetFingerState(0) == Input::InputState::DOWN &&
		m_input->GetTouch()->GetFingerState(1) == Input::InputState::DOWN)
	{
		glm::vec2 halfSize = glm::vec2(0.5f, 0.5f);
		float currDistance0 = glm::length(glm::vec2(m_input->GetTouch()->GetX(0), m_input->GetTouch()->GetY(0)) - halfSize);
		float currDistance1 = glm::length(glm::vec2(m_input->GetTouch()->GetX(1), m_input->GetTouch()->GetY(1)) - halfSize);
		float diff0 = prevDistance0 - currDistance0;
		float diff1 = prevDistance1 - currDistance1;
		float d = diff0 + diff1;
		prevDistance0 = currDistance0;
		prevDistance1 = currDistance1;
		m_graphics->GetCamera()->MoveForward(-d * _runtime.Dt * 100.0f);
	}
	else if (m_input->GetTouch()->GetFingerState(0) == Input::InputState::DOWN)
	{
		m_graphics->GetCamera()->UpdateTouch(m_input->GetTouch()->GetdX((SDL_FingerID)0) * 1500.0f,
											 m_input->GetTouch()->GetdY((SDL_FingerID)0) * 1500.0f);
	}

#else
	if (m_input->GetKeyboard()->IsTextInputActive())
		return;
	/*	Fulhax just nu, enbart för att slippa ha det i gameloopen (REN OCH FIN)	*/

	if (m_input->GetKeyboard()->GetKeyState(SDL_SCANCODE_W) == Input::InputState::DOWN)
		m_graphics->GetCamera()->MoveForward(_runtime.Dt);
	if (m_input->GetKeyboard()->GetKeyState(SDL_SCANCODE_S) == Input::InputState::DOWN)
		m_graphics->GetCamera()->MoveBackward(_runtime.Dt);
	if (m_input->GetKeyboard()->GetKeyState(SDL_SCANCODE_A) == Input::InputState::DOWN)
		m_graphics->GetCamera()->MoveLeft(_runtime.Dt);
	if (m_input->GetKeyboard()->GetKeyState(SDL_SCANCODE_D) == Input::InputState::DOWN)
		m_graphics->GetCamera()->MoveRight(_runtime.Dt);

	// ROTATE CAMERA
	if (m_input->GetMouse()->GetButtonState(Input::RightButton) == Input::InputState::DOWN)
	{
		int sizeX, sizeY;
		m_graphics->GetWindowSize(sizeX, sizeY);

		m_graphics->GetCamera()->UpdateMouse(sizeX*0.5, sizeY*0.5, m_input->GetMouse()->GetX(), m_input->GetMouse()->GetY());
		m_input->GetMouse()->SetPosition(sizeX*0.5, sizeY*0.5);
		m_input->GetMouse()->HideCursor(true);
	}
	else
		m_input->GetMouse()->HideCursor(false);
#endif
}