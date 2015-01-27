#ifndef ECSLPROFILER_H
#define ECSLPROFILER_H

#include <vector>
#include "ECSLFrame.h"
#include "ECSLStatistics.h"
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
		void Update(float _dt);
		void Render();

	private:
		const float DefaultBufferSwapTime = 0.5f;
		const unsigned int TextHeight = 1;

		bool m_active;
		float m_bufferSwapTimer;
		Renderer::GraphicDevice* m_graphics;
		MPL::ThreadLogger* m_threadLogger;
		ECSLStatistics* m_frontBufferStatistics;
		ECSLStatistics* m_backBufferStatistics;

		Profilers::ECSLFrame* CreateFrame();

		Profilers::ECSLFrame* GenerateFrameData(MPL::LoggedSession* _session);

		void RenderWaitingForStats(float _x, float _y);
		void RenderWorkItemStats(float _x, float _y);

		void GetScreenPosition(int& _outX, int& _outY, float _x, float _y);

		void GetTextBoxPosition(int& _outX, int& _outY, int _lines);

		void SwapBuffers();

		void ResetSwapTimer();
	};
}

#endif