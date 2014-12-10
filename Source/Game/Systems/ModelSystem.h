#ifndef MODELSYSTEM_H
#define MODELSYSTEM_H

#include "ECSL/Interfaces/System.h"
#include "Renderer/GraphicDevice.h"


class ModelSystem : public ECSL::System
{
public:
	ModelSystem(Renderer::GraphicDevice* _graphics);
	~ModelSystem();

	void Update(float _dt);
	void Initialize();

	void OnEntityAdded(unsigned int _entityId);
	void OnEntityRemoved(unsigned int _entityId);

private:
	Renderer::GraphicDevice* m_graphics;
};

#endif