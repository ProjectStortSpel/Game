#ifndef ECSLLOADINGVIEW_H
#define ECSLLOADINGVIEW_H

#include "ECSLRenderView.h"

namespace Profilers
{
	class ECSLLoadingView : public ECSLRenderView
	{
	public:
		ECSLLoadingView(Renderer::GraphicDevice* _graphics);
		~ECSLLoadingView();

		void Update(ECSLStatistics* _frontBufferStatistics);

	private:
		const int StartPosX = 0;
		const int StartPosY = 0;
		const int TextHeight = 1;
	};
}

#endif