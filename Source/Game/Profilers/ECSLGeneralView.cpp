#include "ECSLGeneralView.h"

using namespace Profilers;

ECSLGeneralView::ECSLGeneralView(Renderer::GraphicDevice* _graphics)
: ECSLRenderView(_graphics)
{

}

ECSLGeneralView::~ECSLGeneralView()
{

}

void ECSLGeneralView::Update(ECSLStatistics* _frontBufferStatistics)
{
	ClearTextEntries();

	unsigned int x = StartPosX;
	unsigned int y = StartPosY;

	std::stringstream text;
	text << "Benchmark for the Multi-threaded Entity Component System";
	TextEntry* textEntry = new TextEntry();
	textEntry->x = x;
	textEntry->y = y;
	textEntry->text = new std::string(text.str());
	textEntry->height = TextHeight;
	m_textEntries->push_back(textEntry);
	y += TextHeight;

	text = std::stringstream();
	text << "Press C to change to the previous view. Press V to change to the next view.";
	textEntry = new TextEntry();
	textEntry->x = x;
	textEntry->y = y;
	textEntry->text = new std::string(text.str());
	textEntry->height = TextHeight;
	m_textEntries->push_back(textEntry);
	y += TextHeight;

	text = std::stringstream();
	text << "Press F8 to write the currently displayed text to log.";
	textEntry = new TextEntry();
	textEntry->x = x;
	textEntry->y = y;
	textEntry->text = new std::string(text.str());
	textEntry->height = TextHeight;
	m_textEntries->push_back(textEntry);
	y += TextHeight + TextHeight;

	text = std::stringstream();
	text << "Using " << _frontBufferStatistics->GetThreadCount() << " threads";
	textEntry = new TextEntry();
	textEntry->x = x;
	textEntry->y = y;
	textEntry->text = new std::string(text.str());
	textEntry->height = TextHeight;
	m_textEntries->push_back(textEntry);
	y += TextHeight + TextHeight + TextHeight;

	text = std::stringstream();
	text << "Benchmark Type | Average | Min | Max | Difference (Max - Min)";
	textEntry = new TextEntry();
	textEntry->x = x;
	textEntry->y = y;
	textEntry->text = new std::string(text.str());
	textEntry->height = TextHeight;
	m_textEntries->push_back(textEntry);
	y += TextHeight + TextHeight;

	text = std::stringstream();
	text << "Total Time | " << _frontBufferStatistics->GetAvgFrameTime() << " | " << _frontBufferStatistics->GetMinFrameTime() << " | " 
		<< _frontBufferStatistics->GetMaxFrameTime() << " | " << _frontBufferStatistics->GetDiffFrameTime();
	textEntry = new TextEntry();
	textEntry->x = x;
	textEntry->y = y;
	textEntry->text = new std::string(text.str());
	textEntry->height = TextHeight;
	m_textEntries->push_back(textEntry);
	y += TextHeight + TextHeight;

	text = std::stringstream();
	text << "Overall Thread Efficiency | " << _frontBufferStatistics->GetAvgTotalEfficiency() * 100.0f << "% | " << _frontBufferStatistics->GetMinTotalEfficiency() * 100.0f << "% | "
		<< _frontBufferStatistics->GetMaxTotalEfficiency() * 100.0f << "% | " << _frontBufferStatistics->GetDiffTotalEfficiency() * 100.0f << "%";
	textEntry = new TextEntry();
	textEntry->x = x;
	textEntry->y = y;
	textEntry->text = new std::string(text.str());
	textEntry->height = TextHeight;
	m_textEntries->push_back(textEntry);
	y += TextHeight + TextHeight;

	text = std::stringstream();
	text << "Total Work Time | " << _frontBufferStatistics->GetAvgTotalWorkTime() << " | " << _frontBufferStatistics->GetMinTotalWorkTime() << " | "
		<< _frontBufferStatistics->GetMaxTotalWorkTime() << " | " << _frontBufferStatistics->GetDiffTotalWorkTime();
	textEntry = new TextEntry();
	textEntry->x = x;
	textEntry->y = y;
	textEntry->text = new std::string(text.str());
	textEntry->height = TextHeight;
	m_textEntries->push_back(textEntry);
	y += TextHeight + TextHeight;

	text = std::stringstream();
	text << "Total Overhead Time | " << _frontBufferStatistics->GetAvgTotalOverheadTime() << " | " << _frontBufferStatistics->GetMinTotalOverheadTime() << " | "
		<< _frontBufferStatistics->GetMaxTotalOverheadTime() << " | " << _frontBufferStatistics->GetDiffTotalOverheadTime();
	textEntry = new TextEntry();
	textEntry->x = x;
	textEntry->y = y;
	textEntry->text = new std::string(text.str());
	textEntry->height = TextHeight;
	m_textEntries->push_back(textEntry);
	y += TextHeight + TextHeight;

	text = std::stringstream();
	text << "Effective Thread Count | " << _frontBufferStatistics->GetThreadCount() * _frontBufferStatistics->GetAvgTotalEfficiency() << " | " << _frontBufferStatistics->GetThreadCount() * _frontBufferStatistics->GetMinTotalEfficiency() << " | "
		<< _frontBufferStatistics->GetThreadCount() * _frontBufferStatistics->GetMaxTotalEfficiency() << " | " << _frontBufferStatistics->GetThreadCount() * _frontBufferStatistics->GetDiffTotalEfficiency();
	textEntry = new TextEntry();
	textEntry->x = x;
	textEntry->y = y;
	textEntry->text = new std::string(text.str());
	textEntry->height = TextHeight;
	m_textEntries->push_back(textEntry);
	y += TextHeight + TextHeight;
}