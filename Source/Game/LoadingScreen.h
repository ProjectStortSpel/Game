#ifndef LOADINGSCREEN_H
#define LOADINGSCREEN_H

#include <vector>
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
	float m_r, m_g, m_b;

	int m_backgroundModel, m_textModel;
	glm::mat4 m_backgroundMatrix, m_textMatrix;
	
	float m_color[3];
	float m_scaleX, m_scaleY;

	bool m_isActive;
	int m_accessLevel;

public:

	LoadingScreen();
	~LoadingScreen();
	static LoadingScreen& GetInstance();
	void SetGraphicsDevice(Renderer::GraphicDevice* _graphicDevice);

	void SetTextColor(float _r, float _g, float _b);
	void SetLoadingText(std::string _text);
	void SetBackground(std::string _path);
	void SetBackground(std::vector<std::string> _paths, std::string _file);

	void SetActive();
	void SetInactive(int _level);

	void SetNeedForce(int _value) { if (_value > m_accessLevel) m_accessLevel = _value; }

	bool IsActive();
};


#endif