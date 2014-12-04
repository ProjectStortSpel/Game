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
	AddComponentTypeToFilter("Position", ECSL::FilterType::Mandatory);


	printf("CameraSystem initialized!\n");
}

void CameraSystem::Update(float _dt)
{
	if (GetEntities()->size() == 0)
		return;
}

void CameraSystem::OnEntityAdded(unsigned int _entityId)
{

}

void CameraSystem::OnEntityRemoved(unsigned int _entityId)
{

}