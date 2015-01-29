#include "ECSLWorkItemView.h"

using namespace Profilers;

ECSLWorkItemView::ECSLWorkItemView(Renderer::GraphicDevice* _graphics)
: ECSLRenderView(_graphics), m_pageIndex(0), m_pages(0)
{

}

ECSLWorkItemView::~ECSLWorkItemView()
{
	if (m_pages)
	{
		for (auto page : *m_pages)
			delete(page);
		delete(m_pages);
		m_pages = 0;
	}
}

void ECSLWorkItemView::Update(ECSLStatistics* _frontBufferStatistics)
{
	if (!m_pages)
		CreatePages(_frontBufferStatistics);

	ClearTextEntries();

	unsigned int x = StartPosX;
	unsigned int y = StartPosY;

	std::stringstream text;
	text << "Time taken for each work item during each frame";
	TextEntry* textEntry = new TextEntry();
	textEntry->x = x;
	textEntry->y = y;
	textEntry->text = new std::string(text.str());
	textEntry->height = TextHeight;
	m_textEntries->push_back(textEntry);
	y += TextHeight;

	text = std::stringstream();
	text << "Work Item Name | Avg Time | Min Time | Max Time | Difference (Max - Min)";
	textEntry = new TextEntry();
	textEntry->x = x;
	textEntry->y = y;
	textEntry->text = new std::string(text.str());
	textEntry->height = TextHeight;
	m_textEntries->push_back(textEntry);
	y += TextHeight + TextHeight;

	auto workItemGroups = _frontBufferStatistics->GetWorkItemStats();
	unsigned int startIndex = (*m_pages)[m_pageIndex]->startGroupIndex;
	unsigned int endIndex = (*m_pages)[m_pageIndex]->endGroupIndex;
	for (unsigned int i = startIndex; i < endIndex; ++i)
	{
		auto workItemGroup = (*workItemGroups)[i];

		text = std::stringstream();
		text << "Group " << i;
		textEntry = new TextEntry();
		textEntry->x = x;
		textEntry->y = y;
		textEntry->text = new std::string(text.str());
		textEntry->height = TextHeight;
		m_textEntries->push_back(textEntry);
		y += TextHeight;

		for (unsigned int j = 0; j < workItemGroup->size(); ++j)
		{
			auto workItem = workItemGroup->at(j);
			std::stringstream text;
			text << *workItem->name << " | " << workItem->avgDuration << " | " << 
				workItem->minDuration << " | " << workItem->maxDuration <<
				" | " << workItem->diffDuration;

			textEntry = new TextEntry();
			textEntry->x = x;
			textEntry->y = y;
			textEntry->text = new std::string(text.str());
			textEntry->height = TextHeight;
			m_textEntries->push_back(textEntry);

			printf("Name:%s\n", workItem->name->c_str());

			y += TextHeight;
		}

		y += TextHeight;
	}
}

bool ECSLWorkItemView::NextSubview(ECSLStatistics* _frontBufferStatistics)
{
	if (m_pageIndex + 1 == PageCount())
		return false;
	else
	{
		++m_pageIndex;
		return true;
	}
}

bool ECSLWorkItemView::PreviousSubview(ECSLStatistics* _frontBufferStatistics)
{
	if (m_pageIndex - 1 == -1)
		return false;
	else
	{
		--m_pageIndex;
		return true;
	}
}

void ECSLWorkItemView::FirstSubview(ECSLStatistics* _frontBufferStatistics)
{
	if (!m_pages)
		CreatePages(_frontBufferStatistics);

	m_pageIndex = 0;
}

void ECSLWorkItemView::LastSubview(ECSLStatistics* _frontBufferStatistics)
{
	if (!m_pages)
		CreatePages(_frontBufferStatistics);

	m_pageIndex = PageCount() - 1;
}

void ECSLWorkItemView::CreatePages(ECSLStatistics* _frontBufferStatistics)
{
	if (!_frontBufferStatistics)
		return;

	m_pages = new std::vector<WorkItemPage*>();

	WorkItemPage* currentPage = new WorkItemPage();
	currentPage->startGroupIndex = 0;
	currentPage->endGroupIndex = 0;
	float currentPageSize = 0.0f;
	auto workItemGroups = _frontBufferStatistics->GetWorkItemStats();
	/*  
		Iterate through each work item group and try add the group to the current page.
		If the page is full: Create a new page
	*/
	for (auto workItemGroup : *workItemGroups)
	{
		currentPageSize += TextHeight * workItemGroup->size() + TextHeight * 2;
		if (currentPageSize <= PageMaxHeight)
		{
			++currentPage->endGroupIndex;
		}
		else
		{
			WorkItemPage* newPage = new WorkItemPage();
			newPage->startGroupIndex = currentPage->endGroupIndex;
			newPage->endGroupIndex = newPage->startGroupIndex + 1;
			m_pages->push_back(currentPage);
			currentPage = newPage;
			currentPageSize = TextHeight * workItemGroup->size() + TextHeight * 2;
		}
	}
	/* Add last page if there are any groups added to the page. If not, then free memory. */
	if (currentPage->startGroupIndex != currentPage->endGroupIndex)
		m_pages->push_back(currentPage);
	else
		delete(currentPage);
}