#ifndef GRAPHICALSYSTEM_H
#define GRAPHICALSYSTEM_H

#include "ECSL/Interfaces/System.h"
#if defined(__ANDROID__) || defined(__IOS__)
#include "AndroidRenderer/GraphicDevice.h"
#else
#include "Renderer/GraphicDevice.h"
#endif


class GraphicalSystem : public ECSL::System
{
public:
	GraphicalSystem(){}
	void SetGraphics(Renderer::GraphicDevice* _graphics){ m_graphics = _graphics; }

protected:
	Renderer::GraphicDevice* m_graphics;

};

#endif