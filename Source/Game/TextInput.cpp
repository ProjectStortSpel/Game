#include "Game/TextInput.h"

TextInput::TextInput()
{
	INPUT = &Input::InputWrapper::GetInstance();
	m_text = "";
}

TextInput::~TextInput()
{

}

void TextInput::Update()
{
	if (m_active)
	{
		if (INPUT->GetKeyboard()->GetKeyState(SDL_SCANCODE_RETURN) == Input::InputState::PRESSED ||
			INPUT->GetKeyboard()->GetKeyState(SDL_SCANCODE_KP_ENTER) == Input::InputState::PRESSED)
		{
			if (m_hook)
				m_hook(m_text.c_str());
			INPUT->GetKeyboard()->ResetTextInput();			
		}

		m_text = INPUT->GetKeyboard()->GetTextInput();
	}
}

void TextInput::SetActive(bool _active)
{
	if (_active)
	{
		INPUT->GetKeyboard()->SetTextInput(m_text.c_str());
	}
	m_active = _active;
}