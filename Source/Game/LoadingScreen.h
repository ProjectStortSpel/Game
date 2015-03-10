#ifndef LOADINGSCREEN_H
#define LOADINGSCREEN_H

#include <string>

#if defined(__IOS__) || defined(__ANDROID__)
#include "AndroidRenderer/GraphicDevice.h"
#else
#include "Renderer/GraphicDevice.h"
#endif

class LoadingScreen
{
private:
	Renderer::GraphicDevice* m_graphicDevice;
	char m_r, m_g, m_b;

	int m_backgroundModel, m_textModel;

public:

	LoadingScreen();
	~LoadingScreen();
	static LoadingScreen& GetInstance();
	void SetGraphicsDevice(Renderer::GraphicDevice* _graphicDevice);

	void SetTextColor(char r, char g, char b);
	void SetLoadingText(std::string _text);
};


#endif