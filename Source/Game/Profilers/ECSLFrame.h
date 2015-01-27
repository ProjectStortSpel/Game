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
			unsigned int threadId;
			float duration;

			WorkItemStatistic() : name(0), groupId(0), duration(0.0f) { }
			~WorkItemStatistic() { delete(name); }
		};

		ECSLFrame(unsigned int _threadCount, float _frameTime);
		~ECSLFrame();

		void AddOverheadData(MPL::LoggedAction* _action, unsigned int _threadId);
		void AddWorkData(MPL::LoggedAction* _action, unsigned int _threadId);
		void AddWorkItemStatistic(MPL::LoggedAction* _action, unsigned int _threadId);
		void CalculateEfficiencyData(unsigned int _threadCount);

		unsigned int GetThreadCount() { return m_threadCount; }
		float GetFrameTime() { return m_frameTime; }
		float GetTotalEfficiency() { return m_totalEfficiency; }
		float* GetThreadEfficiency() { return m_threadEfficiency; }
		float GetTotalWorkTime() { return m_totalWorkTime; }
		float* GetThreadWorkTime() { return m_threadWorkTime; }
		float GetTotalOverheadTime() { return m_totalOverheadTime; }
		float* GetThreadOverheadTime() { return m_threadOverheadTime; }
		const std::vector<WorkItemStatistic*>* GetWorkItemStats() { return m_workItemStats; }

		void SetFrameTime(float _frameTime) { m_frameTime = _frameTime; }

	private:
		unsigned int m_threadCount;
		float m_frameTime;
		float m_totalEfficiency;
		float* m_threadEfficiency;
		float m_totalWorkTime;
		float* m_threadWorkTime;
		float m_totalOverheadTime;
		float* m_threadOverheadTime;
		std::vector<WorkItemStatistic*>* m_workItemStats;
	};

	/* Create a float array with all values set to zero */
	float* FloatArray(unsigned int _size);
}

#endif