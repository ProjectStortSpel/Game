#ifndef ECSLSTATISTICS_H
#define ECSLSTATISTICS_H

#include "ECSLFrame.h"

namespace Profilers
{
	struct ECSLStatistics
	{
	public:
		ECSLStatistics(unsigned int _threadCount);
		~ECSLStatistics();

		void AddFrame(ECSLFrame* _frame);

	private:
		unsigned int m_threadCount;

		float m_avgFrameTime;
		float m_minFrameTime;
		float m_maxFrameTime;
		float m_diffFrameTime;

		float m_avgTotalEfficiency;
		float m_minTotalEfficiency;
		float m_maxTotalEfficiency;
		float m_diffTotalEfficiency;

		float* m_avgThreadEfficiency;
		float* m_minThreadEfficiency;
		float* m_maxThreadEfficiency;
		float* m_diffThreadEfficiency;

		float m_avgTotalWorkTime;
		float m_minTotalWorkTime;
		float m_maxTotalWorkTime; 
		float m_diffTotalWorkTime;

		float* m_avgThreadWorkTime;
		float* m_minThreadWorkTime;
		float* m_maxThreadWorkTime;
		float* m_diffThreadWorkTime;

		float m_avgTotalOverheadTime;
		float m_minTotalOverheadTime;
		float m_maxTotalOverheadTime;
		float m_diffTotalOverheadTime;

		float* m_avgThreadOverheadTime;
		float* m_minThreadOverheadTime;
		float* m_maxThreadOverheadTime;
		float* m_diffThreadOverheadTime;

		std::vector<std::vector<ECSLFrame::WorkItemStatistic*>*>* m_workItemStats;

		std::vector<ECSLFrame*>* m_frames;
	};
}

#endif