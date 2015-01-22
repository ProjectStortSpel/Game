#ifndef MAPISLANDCREATORSYSTEM_H
#define MAPISLANDCREATORSYSTEM_H

#include "ECSL/Interfaces/System.h"
#ifdef __ANDROID__
#include "AndroidRenderer/GraphicDevice.h"
#else
#include "Renderer/GraphicDevice.h"
#endif

class MapIslandCreatorSystem : public ECSL::System
{
public:
	MapIslandCreatorSystem();
	~MapIslandCreatorSystem();

	void Update(float _dt);
	void Initialize();

	void OnEntityAdded(unsigned int _entityId);
	void OnEntityRemoved(unsigned int _entityId);
};

#endif