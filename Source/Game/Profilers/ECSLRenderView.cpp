#include "ECSLRenderView.h"

#include "Game/Logger/Logger.h"

using namespace Profilers;

ECSLRenderView::ECSLRenderView()
{

}

ECSLRenderView::~ECSLRenderView()
{

}

void ECSLRenderView::WriteToLog()
{
	std::string message = "";
	for (auto textEntry : *m_textEntries)
		message += *textEntry.text + "\n";

	Logger::GetInstance().Log(Logger::GetInstance().AddGroup("World Profiler", false), LogSeverity::Info, message);
}