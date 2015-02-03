#ifndef ECSLRENDERVIEW_H
#define ECSLRENDERVIEW_H

#include <vector>
#include "ECSLStatistics.h"
#ifdef __ANDROID__
#include "AndroidRenderer/GraphicDevice.h"
#else
#include "Renderer/GraphicDevice.h"
#endif

namespace Profilers
{
	struct TextEntry
	{
		std::string* text;
		int x;
		int y;
		int height;

		~TextEntry() { delete(text); }
	};

	class ECSLRenderView
	{
	public:
		ECSLRenderView();
		ECSLRenderView(Renderer::GraphicDevice* _graphics);
		virtual ~ECSLRenderView() = 0;

		void Display();

		void ClearTextEntries();

		virtual void Update(ECSLStatistics* _frontBufferStatistics) = 0;
		virtual bool NextSubview(ECSLStatistics* _frontBufferStatistics);
		virtual bool PreviousSubview(ECSLStatistics* _frontBufferStatistics);
		virtual void FirstSubview(ECSLStatistics* _frontBufferStatistics);
		virtual void LastSubview(ECSLStatistics* _frontBufferStatistics);

		void WriteToLog();

		const std::vector<TextEntry*>* GetTextEntries() { return m_textEntries; }
		
	protected:
		Renderer::GraphicDevice* m_graphics;

		std::vector<TextEntry*>* m_textEntries;

	};
}

#endif