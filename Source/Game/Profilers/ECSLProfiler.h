#ifndef ECSLPROFILER_H
#define ECSLPROFILER_H

#include <vector>
#include "ECSLFrame.h"
#include "ECSLStatistics.h"
#include "ECSLRenderView.h"
#include "ECSL/Interfaces/System.h"
#ifdef __ANDROID__
#include "AndroidRenderer/GraphicDevice.h"
#else
#include "Renderer/GraphicDevice.h"
#endif

namespace Profilers
{
	class ECSLProfiler
	{
	public:
		ECSLProfiler(Renderer::GraphicDevice* _graphics);
		~ECSLProfiler();

		void Toggle();
		void NextView();
		void PreviousView();
		void WriteToLog();
		void Begin();
		void End();
		void Update(float _dt);
		void Render();

	private:
		enum State
		{
			INACTIVE,
			WAITING_FOR_STATISTICS,
			RENDERING
		};

		const float DefaultBufferSwapTime = 0.5f;
		const unsigned int TextHeight = 1;

		State m_state;
		float m_bufferSwapTimer;
		Renderer::GraphicDevice* m_graphics;
		ECSLFrame* m_currentFrame;
		MPL::ThreadLogger* m_threadLogger;
		ECSLStatistics* m_frontBufferStatistics;
		ECSLStatistics* m_backBufferStatistics;
		unsigned int m_renderViewIndex;
		ECSLRenderView* m_renderView;
		std::vector<ECSLRenderView*>* m_renderViews;

		void CreateRenderViews();

		Profilers::ECSLFrame* CreateFrame();

		Profilers::ECSLFrame* GenerateFrameData(MPL::LoggedSession* _session);

		void SwapBuffers();

		void ResetSwapTimer();
	};
}

#endif