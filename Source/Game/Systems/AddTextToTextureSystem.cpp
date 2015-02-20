#include "AddTextToTextureSystem.h"

AddTextToTextureSystem::AddTextToTextureSystem(Renderer::GraphicDevice* _graphics)
{
	m_graphics = _graphics;
}

void AddTextToTextureSystem::Initialize()
{
	SetSystemName("AddTextToTextureSystem");

	SetEntitiesAddedTaskCount(1);

	AddComponentTypeToFilter("TextTexture",	ECSL::FilterType::Mandatory);
	AddComponentTypeToFilter("Model", ECSL::FilterType::Mandatory);
	AddComponentTypeToFilter("Render", ECSL::FilterType::Excluded);
	AddComponentTypeToFilter("Scale", ECSL::FilterType::Mandatory);
}

void AddTextToTextureSystem::PostInitialize()
{
	m_graphics->AddFont("content/fonts/verdanab.ttf", 72);
}

void AddTextToTextureSystem::EntitiesAdded(const ECSL::RuntimeInfo& _runtime, const std::vector<unsigned int>& _entities)
{
	for (unsigned int entityId : _entities)
	{
		int* modelId = (int*)GetComponent(entityId, "Render", "ModelId");
		const char* N = (char*)GetComponent(entityId, "TextTexture", "Name");
		const char* Text = (const char*)GetComponent(entityId, "TextTexture", "Text");
		int* FontIndex = (int*)GetComponent(entityId, "TextTexture", "FontIndex");
		float* R = (float*)GetComponent(entityId, "TextTexture", "R");
		float* G = (float*)GetComponent(entityId, "TextTexture", "G");
		float* B = (float*)GetComponent(entityId, "TextTexture", "B");
		SDL_Color color = { static_cast<Uint8>(255 * *R), static_cast<Uint8>(255 * *G), static_cast<Uint8>(255 * *B), 0 };
		float ratio = m_graphics->CreateTextTexture(N, Text, *FontIndex, color);
		float* scalex = (float*)GetComponent(entityId, "Scale", 0);
		float* scaley = (float*)GetComponent(entityId, "Scale", 4);
		float* scalez = (float*)GetComponent(entityId, "Scale", 8);
		if (*scaley * ratio > *scalex)
			*scaley = *scalex / ratio;
		else
			*scalex = *scaley * ratio;
		m_graphics->ChangeModelTexture(*modelId, N);
	}
}