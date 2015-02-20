#include "ECSLLoadingView.h"

using namespace Profilers;

ECSLLoadingView::ECSLLoadingView(Renderer::GraphicDevice* _graphics)
: ECSLRenderView(_graphics)
{

}

ECSLLoadingView::~ECSLLoadingView()
{

}

void ECSLLoadingView::Update(ECSLStatistics* _frontBufferStatistics)
{
	ClearTextEntries();

	unsigned int x = StartPosX;
	unsigned int y = StartPosY;

	std::stringstream text;
	text << "Collecting data...";
	TextEntry* textEntry = new TextEntry();
	textEntry->x = x;
	textEntry->y = y;
	textEntry->text = new std::string(text.str());
	textEntry->height = TextHeight;
	m_textEntries->push_back(textEntry);
}