#ifndef ECSLGENERALVIEW_H
#define ECSLGENERALVIEW_H

#include "ECSLRenderView.h"

namespace Profilers
{
	class ECSLGeneralView : public ECSLRenderView
	{
	public:
		ECSLGeneralView(Renderer::GraphicDevice* _graphics);
		~ECSLGeneralView();

		void Update(ECSLStatistics* _frontBufferStatistics);

	private:
		const int StartPosX = 0;
		const int StartPosY = 0;
		const int TextHeight = 1;
	};
}

#endif