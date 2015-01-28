#include "ECSLThreadView.h"

using namespace Profilers;

ECSLThreadView::ECSLThreadView(Renderer::GraphicDevice* _graphics)
: ECSLRenderView(_graphics)
{

}

ECSLThreadView::~ECSLThreadView()
{

}

void ECSLThreadView::Update(ECSLStatistics* _frontBufferStatistics)
{
	ClearTextEntries();

	unsigned int x = StartPosX;
	unsigned int y = StartPosY;

	std::stringstream text;
	text << "Thread ID | Average | Min | Max | Difference (Max - Min)";
	TextEntry* textEntry = new TextEntry();
	textEntry->x = x;
	textEntry->y = y;
	textEntry->text = new std::string(text.str());
	textEntry->height = TextHeight;
	m_textEntries->push_back(textEntry);
	y += TextHeight + TextHeight;

	text = std::stringstream();
	text << "Efficiency";
	textEntry = new TextEntry();
	textEntry->x = x;
	textEntry->y = y;
	textEntry->text = new std::string(text.str());
	textEntry->height = TextHeight;
	m_textEntries->push_back(textEntry);
	y += TextHeight;

	for (unsigned int i = 0; i < _frontBufferStatistics->GetThreadCount(); ++i)
	{
		const float* avg = _frontBufferStatistics->GetAvgThreadEfficiency();
		const float* min = _frontBufferStatistics->GetMinThreadEfficiency();
		const float* max = _frontBufferStatistics->GetMaxThreadEfficiency();
		const float* diff = _frontBufferStatistics->GetDiffThreadEfficiency();

		text = std::stringstream();
		text << i << " | " << avg[i] * 100.0f << "% | " << min[i] * 100.0f << "% | "
			<< max[i] * 100.0f << "% | " << diff[i] * 100.0f << "%";
		textEntry = new TextEntry();
		textEntry->x = x;
		textEntry->y = y;
		textEntry->text = new std::string(text.str());
		textEntry->height = TextHeight;
		m_textEntries->push_back(textEntry);
		y += TextHeight;
	}
	y += TextHeight;

	text = std::stringstream();
	text << "Work Time";
	textEntry = new TextEntry();
	textEntry->x = x;
	textEntry->y = y;
	textEntry->text = new std::string(text.str());
	textEntry->height = TextHeight;
	m_textEntries->push_back(textEntry);
	y += TextHeight;

	for (unsigned int i = 0; i < _frontBufferStatistics->GetThreadCount(); ++i)
	{
		const float* avg = _frontBufferStatistics->GetAvgThreadWorkTime();
		const float* min = _frontBufferStatistics->GetMinThreadWorkTime();
		const float* max = _frontBufferStatistics->GetMaxThreadWorkTime();
		const float* diff = _frontBufferStatistics->GetDiffThreadWorkTime();

		text = std::stringstream();
		text << i << " | " << avg[i] << " | " << min[i] << " | "
			<< max[i] << " | " << diff[i];
		textEntry = new TextEntry();
		textEntry->x = x;
		textEntry->y = y;
		textEntry->text = new std::string(text.str());
		textEntry->height = TextHeight;
		m_textEntries->push_back(textEntry);
		y += TextHeight;
	}
	y += TextHeight;

	text = std::stringstream();
	text << "Overhead Time";
	textEntry = new TextEntry();
	textEntry->x = x;
	textEntry->y = y;
	textEntry->text = new std::string(text.str());
	textEntry->height = TextHeight;
	m_textEntries->push_back(textEntry);
	y += TextHeight;

	for (unsigned int i = 0; i < _frontBufferStatistics->GetThreadCount(); ++i)
	{
		const float* avg = _frontBufferStatistics->GetAvgThreadOverheadTime();
		const float* min = _frontBufferStatistics->GetMinThreadOverheadTime();
		const float* max = _frontBufferStatistics->GetMaxThreadOverheadTime();
		const float* diff = _frontBufferStatistics->GetDiffThreadOverheadTime();

		text = std::stringstream();
		text << i << " | " << avg[i] << " | " << min[i] << " | "
			<< max[i] << " | " << diff[i];
		textEntry = new TextEntry();
		textEntry->x = x;
		textEntry->y = y;
		textEntry->text = new std::string(text.str());
		textEntry->height = TextHeight;
		m_textEntries->push_back(textEntry);
		y += TextHeight;
	}
	y += TextHeight;
}