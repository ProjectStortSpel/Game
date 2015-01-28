#ifndef ECSLTHREADVIEW_H
#define ECSLTHREADVIEW_H

#include "ECSLRenderView.h"

namespace Profilers
{
	class ECSLThreadView : public ECSLRenderView
	{
	public:
		ECSLThreadView(Renderer::GraphicDevice* _graphics);
		~ECSLThreadView();

		void Update(ECSLStatistics* _frontBufferStatistics);

	private:
		const int StartPosX = 0;
		const int StartPosY = 0;
		const int TextHeight = 1.0f;
	};
}

#endif