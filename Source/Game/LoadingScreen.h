#ifndef LOADINGSCREEN_H
#define LOADINGSCREEN_H

#include <string>
#include "Renderer/GraphicDevice.h"

class LoadingScreen
{
private:
	Renderer::GraphicDevice* m_graphicDevice;
	char m_r, m_g, m_b;

public:

	LoadingScreen();
	~LoadingScreen();
	static LoadingScreen& GetInstance();
	void SetGraphicsDevice(Renderer::GraphicDevice* _graphicDevice);

	void SetTextColor(char r, char g, char b);
	void SetLoadingText(std::string _text);
};


#endif