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

		/* Activate/Deactivate the profiler. Data will not be collected while deactivated. */
		void Toggle();

		/* Switch to the next render view */
		void NextView();

		/* Switch to the previous render view */
		void PreviousView();

		/* Write the currently displayed statistics to log */
		void LogDisplayedStatistics();

		/* Begin gathering thread data */
		void Begin();

		/* Stop gathering thread data */
		void End();

		/* Use the collected data to create statistics */
		void Update(float _dt);

		/* Render statistics */
		void Render();

	private:
		enum State
		{
			INACTIVE,
			COLLECTING_DATA,
			SHOWING_STATISTICS
		};

		const float DefaultBufferSwapTime = 1.0f;

		State m_state;
		float m_bufferSwapTimer;
		Renderer::GraphicDevice* m_graphics;
		ECSLFrame* m_currentFrame;
		MPL::ThreadLogger* m_threadLogger;
		ECSLStatistics* m_frontBufferStatistics;
		ECSLStatistics* m_backBufferStatistics;
		unsigned int m_renderViewIndex;
		ECSLRenderView* m_loadingView;
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