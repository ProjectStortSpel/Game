#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H

#include "ECSL/Interfaces/System.h"


class RenderSystem : public ECSL::System
{
public:
	RenderSystem();
	~RenderSystem();
	LOLSYSTEM(Renderer::GraphicDevice* _graphics) { m_graphics = _graphics; lol = false; }
	~LOLSYSTEM() { }

	void Update(float _dt)
	{


		auto entites = *GetEntities();
		for (auto entity : entites)
		{
			float *Xp, *Yp, *Zp;
			Xp = (float*)GetComponent((unsigned int)entity, 0, 0);
			Yp = (float*)GetComponent(entity, "Position", "Y");
			Zp = (float*)GetComponent(entity, "Position", "Z");

			*Xp += 0.6f * _dt;

			glm::mat4* eMat = (glm::mat4*)GetComponent(entity, "Render", "Mat");
			*eMat = glm::translate(glm::vec3(*Xp, *Yp, *Zp));

		}

		//m_graphics->RenderSimpleText("")
	}
	void Initialize()
	{
		AddComponentTypeToFilter("Position", ECSL::FilterType::Mandatory);
		AddComponentTypeToFilter("Velocity", ECSL::FilterType::Mandatory);
		AddComponentTypeToFilter("Render", ECSL::FilterType::Mandatory);
		//AddComponentTypeToFilter("Velocity", ECSL::ComponentFilter::RequiresOneOf);
		//AddComponentTypeToFilter("Position", ECSL::ComponentFilter::Excluded);

		printf("Testsystem Initialize()\n");
	}

	void OnEntityAdded(unsigned int _entityId)
	{
		printf("Testsystem OnLOLSYSTEMLOLSYSTEMLOLSYSTEMLOLSYSTEMEntityAdded()\n");
		int* eModelId = (int*)GetComponent(_entityId, "Render", "ModelId");
		glm::mat4* eMat = (glm::mat4*)GetComponent(_entityId, "Render", "Mat");
		*eMat = glm::translate(glm::vec3(1));

		if (!lol)
		{
			*eModelId = m_graphics->LoadModel("content/models/cube/", "cube.object", eMat);
			int b = *eModelId;
			int a = 2;
		}
		else
		{
			*eModelId = m_graphics->LoadModel("content/models/cube2/", "cube2.object", eMat);
			int b = *eModelId;
			int a = 2;
		}
		lol = true;
	}
	void OnEntityRemoved(unsigned int _entityId)
	{
		printf("Testsystem OnEntityRemoved()\n");

		/*	PLocka ut modelid -> säga till grafiken att det idet är inaktivt	*/
	}
private:
	Renderer::GraphicDevice* m_graphics;


	bool lol;
};

#endif