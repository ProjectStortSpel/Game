#ifndef ECSLWORKITEMVIEW_H
#define ECSLWORKITEMVIEW_H

#include "ECSLRenderView.h"

namespace Profilers
{
	class ECSLWorkItemView : public ECSLRenderView
	{
	public:
		ECSLWorkItemView(Renderer::GraphicDevice* _graphics);
		~ECSLWorkItemView();

		void Update(ECSLStatistics* _frontBufferStatistics);

		bool NextSubview(ECSLStatistics* _frontBufferStatistics);
		bool PreviousSubview(ECSLStatistics* _frontBufferStatistics);
		void FirstSubview(ECSLStatistics* _frontBufferStatistics);
		void LastSubview(ECSLStatistics* _frontBufferStatistics);

	private:
		struct WorkItemPage
		{
			unsigned int startGroupIndex;
			unsigned int endGroupIndex;
		};

		const int StartPosX = 0;
		const int StartPosY = 0;
		const int TextHeight = 1;
		const int PageMaxHeight = 41; //44;

		unsigned int m_pageIndex;
		std::vector<WorkItemPage*>* m_pages;

		void CreatePages(ECSLStatistics* _frontBufferStatistics);

		unsigned int PageCount() { return m_pages->size(); }
		void NextPosition(unsigned int& x, unsigned int& y);
	};
}

#endif