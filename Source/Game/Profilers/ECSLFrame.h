#ifndef ECSLFRAME_H
#define ECSLFRAME_H

#include <unordered_map>
#include <vector>
#include "MPL/Managers/ThreadLogger.h"

namespace Profilers
{
	struct ECSLFrame
	{
	public:
		struct WorkItemStatistic
		{
			std::string* name;
			unsigned int groupId;
			float duration;
		};

		ECSLFrame(unsigned int _threadCount, float _frameTime);
		~ECSLFrame();

		void AddOverheadData(MPL::LoggedAction* _action, unsigned int _threadId);
		void AddWorkData(MPL::LoggedAction* _action, unsigned int _threadId);
		void AddWorkItemStatistic(MPL::LoggedAction* _action, unsigned int _threadId);
		void CalculateEfficiencyData(unsigned int _threadCount);

		float GetFrameTime() { return m_frameTime; }
		float GetTotalEfficiency() { return m_totalEfficiency; }
		float* GetThreadEfficiency() { return m_threadEfficiency; }
		float GetTotalWorkTime() { return m_totalWorkTime; }
		float* GetThreadWorkTime() { return m_threadWorkTime; }
		float GetTotalOverheadTime() { return m_totalOverheadTime; }
		float* GetThreadOverheadTime() { return m_threadOverheadTime; }
		const std::unordered_map<unsigned int, std::vector<WorkItemStatistic*>*>* GetWorkItemStats() { return m_workItemStats; }

		void SetFrameTime(float _frameTime) { m_frameTime = _frameTime; }

	private:
		float m_frameTime;
		float m_totalEfficiency;
		float* m_threadEfficiency;
		float m_totalWorkTime;
		float* m_threadWorkTime;
		float m_totalOverheadTime;
		float* m_threadOverheadTime;
		std::unordered_map<unsigned int, std::vector<WorkItemStatistic*>*>* m_workItemStats;
	};

	/* Create a float array with all values set to zero */
	float* FloatArray(unsigned int _size);
}

#endif