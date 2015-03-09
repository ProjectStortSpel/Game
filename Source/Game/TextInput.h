#ifndef TEXTINPUT_H
#define TEXTINPUT_H

#include <string>
#include <functional>
#include "Input/InputWrapper.h"

typedef std::function<void(const char*, bool)> TextHook;

class TextInput
{
private:

	bool m_active;
	std::string m_text;
	TextHook m_hook;

	Input::InputWrapper* INPUT;

public:

	TextInput();
	~TextInput();

	void Update();

	void SetActive(bool _active);
	void SetText(char* _text) { m_text = _text; }
	const char* GetText() { return m_text.c_str(); }
	
	void SetTextHook(TextHook _hook) { m_hook = _hook; }

};


#endif