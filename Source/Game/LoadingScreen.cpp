#include "LoadingScreen.h"
#include "Console/Console.h"

LoadingScreen::LoadingScreen()
{
	m_r = 0;
	m_g = 0;
	m_b = 0;
	m_graphicDevice = 0;


	//m_backgroundModel = m_graphicDevice->LoadModel()
	//m_textModel = m_graphicDevice->LoadModel()
}

LoadingScreen::~LoadingScreen()
{
}

LoadingScreen& LoadingScreen::GetInstance()
{
	static LoadingScreen instance;
	return instance;
}

void LoadingScreen::SetGraphicsDevice(Renderer::GraphicDevice* _graphicDevice)
{
	m_graphicDevice = _graphicDevice;
}

void LoadingScreen::SetTextColor(char r, char g, char b)
{
	m_r = r;
	m_g = g;
	m_b = b;
}

void LoadingScreen::SetLoadingText(std::string _text)
{
	//SDL_Color color = { static_cast<Uint8>(255 * m_r), static_cast<Uint8>(255 * m_g), static_cast<Uint8>(255 * m_b), 0 };
	//m_graphicDevice->CreateTextTexture("loadingtext", _text, 0, color);
	SDL_Log("LoadingScreen: %s", _text.c_str());
	Console::ConsoleManager::GetInstance().AddMessage(_text.c_str());
}