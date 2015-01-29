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
		struct WorkItem
		{
			std::string* name;
			unsigned int localGroupId;
			unsigned int groupId;
			float duration;

			WorkItem() : name(0), groupId(0), duration(0.0f) { }
			~WorkItem() { delete(name); }
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
		const std::vector<std::vector<ECSLFrame::WorkItem*>*>* GetWorkItems() { return m_workItems; }

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
		std::vector<std::vector<ECSLFrame::WorkItem*>*>* m_workItems;
	};

	/* Create a float array with all values set to zero */
	float* FloatArray(unsigned int _size, float _initValue);
}

#endif