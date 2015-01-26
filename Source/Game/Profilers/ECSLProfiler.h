#ifndef ECSLPROFILER_H
#define ECSLPROFILER_H

#include <vector>
#include "ECSLFrame.h"
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
		void OrganizeData();
		void NewFrame();
		void Render();

	private:
		bool m_active;
		Renderer::GraphicDevice* m_graphics;

		Profilers::ECSLFrame* GenerateFrameData(MPL::LoggedSession* _session);

		void RenderAverageEfficiency(float _x, float _y);

		void GetScreenPosition(int _outX, int _outY, float _x, float _y);

	};
}

#endif