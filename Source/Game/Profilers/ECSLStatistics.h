#ifndef ECSLSTATISTICS_H
#define ECSLSTATISTICS_H

#include "ECSLFrame.h"

namespace Profilers
{
	struct ECSLStatistics
	{
	public:
		struct WorkItemStatistic
		{
			std::string* name;
			float avgDuration;
			float minDuration;
			float maxDuration;
			float diffDuration;

			WorkItemStatistic() : name(0) { }
			~WorkItemStatistic() { delete(name); }
		};

		ECSLStatistics(unsigned int _threadCount);
		~ECSLStatistics();

		void AddFrame(ECSLFrame* _frame);

		unsigned int GetThreadCount() { return m_threadCount; }

		const float GetAvgFrameTime() { return m_avgFrameTime; }
		const float GetMinFrameTime() { return m_minFrameTime; }
		const float GetMaxFrameTime() { return m_maxFrameTime; }
		const float GetDiffFrameTime() { return m_diffFrameTime; }

		const float GetAvgTotalEfficiency() { return m_avgTotalEfficiency; }
		const float GetMinTotalEfficiency() { return m_minTotalEfficiency; }
		const float GetMaxTotalEfficiency() { return m_maxTotalEfficiency; }
		const float GetDiffTotalEfficiency() { return m_diffTotalEfficiency; }

		const float* GetAvgThreadEfficiency() { return m_avgThreadEfficiency; }
		const float* GetMinThreadEfficiency() { return m_minThreadEfficiency; }
		const float* GetMaxThreadEfficiency() { return m_maxThreadEfficiency; }
		const float* GetDiffThreadEfficiency() { return m_diffThreadEfficiency; }

		const float GetAvgTotalWorkTime() { return m_avgTotalWorkTime; }
		const float GetMinTotalWorkTime() { return m_minTotalWorkTime; }
		const float GetMaxTotalWorkTime() { return m_maxTotalWorkTime; }
		const float GetDiffTotalWorkTime() { return m_diffTotalWorkTime; }

		const float* GetAvgThreadWorkTime() { return m_avgThreadWorkTime; }
		const float* GetMinThreadWorkTime() { return m_minThreadWorkTime; }
		const float* GetMaxThreadWorkTime() { return m_maxThreadWorkTime; }
		const float* GetDiffThreadWorkTime() { return m_diffThreadWorkTime; }

		const float GetAvgTotalOverheadTime() { return m_avgTotalOverheadTime; }
		const float GetMinTotalOverheadTime() { return m_minTotalOverheadTime; }
		const float GetMaxTotalOverheadTime() { return m_maxTotalOverheadTime; }
		const float GetDiffTotalOverheadTime() { return m_diffTotalOverheadTime; }

		const float* GetAvgThreadOverheadTime() { return m_avgThreadOverheadTime; }
		const float* GetMinThreadOverheadTime() { return m_minThreadOverheadTime; }
		const float* GetMaxThreadOverheadTime() { return m_maxThreadOverheadTime; }
		const float* GetDiffThreadOverheadTime() { return m_diffThreadOverheadTime; }

		const std::vector<std::vector<WorkItemStatistic*>*>* GetWorkItemStats() { return m_workItemStats; }

	private:
		unsigned int m_threadCount;
		unsigned int m_frameCount;

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

		std::vector<std::vector<WorkItemStatistic*>*>* m_workItemStats;
	};
}

#endif