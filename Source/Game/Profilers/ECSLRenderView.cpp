#include "ECSLRenderView.h"

#include "Logger/Managers/Logger.h"

using namespace Profilers;

ECSLRenderView::ECSLRenderView()
{

}

ECSLRenderView::ECSLRenderView(Renderer::GraphicDevice* _graphics)
: m_graphics(_graphics), m_textEntries(new std::vector<TextEntry*>())
{
	
}

ECSLRenderView::~ECSLRenderView()
{
	for (auto textEntry : *m_textEntries)
		delete(textEntry);
	delete(m_textEntries);
}

void ECSLRenderView::Display()
{
	for (TextEntry* textEntry : *m_textEntries)
	{
		m_graphics->RenderSimpleText(*textEntry->text, textEntry->x, textEntry->y);
	}

}

void ECSLRenderView::ClearTextEntries()
{
	for (auto textEntry : *m_textEntries)
		delete(textEntry);
	m_textEntries->clear();
}


bool ECSLRenderView::NextSubview(ECSLStatistics* _frontBufferStatistics)
{
	return false;
}

bool ECSLRenderView::PreviousSubview(ECSLStatistics* _frontBufferStatistics)
{
	return false;
}

void ECSLRenderView::FirstSubview(ECSLStatistics* _frontBufferStatistics)
{

}

void ECSLRenderView::LastSubview(ECSLStatistics* _frontBufferStatistics)
{

}

void ECSLRenderView::WriteToLog()
{
	std::string message = "\r\n";

	for (auto textEntry : *m_textEntries)
		message += *textEntry->text + "\r\n";

	Logger::GetInstance().Log(Logger::GetInstance().AddGroup("World Profiler", false), LogSeverity::Info, message);
}