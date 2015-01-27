#ifndef ECSLRENDERVIEW_H
#define ECSLRENDERVIEW_H

#include <vector>
#include "ECSLStatistics.h"

namespace Profilers
{
	struct TextEntry
	{
		std::string* text;
		int x;
		int y;
	};

	class ECSLRenderView
	{
	public:
		ECSLRenderView();
		virtual ~ECSLRenderView() = 0;

		virtual void CreateView(ECSLStatistics* _frontBufferStatistics) = 0;

		void WriteToLog();

		const std::vector<Profilers::TextEntry>* GetTextEntries() { return m_textEntries; }

	protected:
		std::vector<Profilers::TextEntry>* m_textEntries;

	};
}

#endif