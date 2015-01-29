#include "ECSLStatistics.h"

#include <float.h>

using namespace Profilers;

ECSLStatistics::ECSLStatistics(unsigned int _threadCount)
{
	m_threadCount = _threadCount;
	m_frameCount = 0;

	m_avgFrameTime = 0.0f;
	m_minFrameTime = FLT_MAX;
	m_maxFrameTime = -FLT_MAX;
	m_diffFrameTime = 0.0f;

	m_avgTotalEfficiency = 0.0f;
	m_minTotalEfficiency = FLT_MAX;
	m_maxTotalEfficiency = -FLT_MAX;
	m_diffTotalEfficiency = 0.0f;

	m_avgThreadEfficiency = FloatArray(_threadCount, 0.0f);
	m_minThreadEfficiency = FloatArray(_threadCount, FLT_MAX);
	m_maxThreadEfficiency = FloatArray(_threadCount, -FLT_MAX);
	m_diffThreadEfficiency = FloatArray(_threadCount, 0.0f);

	m_avgTotalWorkTime = 0.0f;
	m_minTotalWorkTime = FLT_MAX;
	m_maxTotalWorkTime = -FLT_MAX;
	m_diffTotalWorkTime = 0.0f;

	m_avgThreadWorkTime = FloatArray(_threadCount, 0.0f);
	m_minThreadWorkTime = FloatArray(_threadCount, FLT_MAX);
	m_maxThreadWorkTime = FloatArray(_threadCount, -FLT_MAX);
	m_diffThreadWorkTime = FloatArray(_threadCount, 0.0f);

	m_avgTotalOverheadTime = 0.0f;
	m_minTotalOverheadTime = FLT_MAX;
	m_maxTotalOverheadTime = -FLT_MAX;
	m_diffTotalOverheadTime = 0.0f;

	m_avgThreadOverheadTime = FloatArray(_threadCount, 0.0f);
	m_minThreadOverheadTime = FloatArray(_threadCount, FLT_MAX);
	m_maxThreadOverheadTime = FloatArray(_threadCount, -FLT_MAX);
	m_diffThreadOverheadTime = FloatArray(_threadCount, 0.0f);

	m_workItemStats = 0;
}

ECSLStatistics::~ECSLStatistics()
{
	delete(m_avgThreadEfficiency);
	delete(m_minThreadEfficiency);
	delete(m_maxThreadEfficiency);
	delete(m_diffThreadEfficiency);

	delete(m_avgThreadWorkTime);
	delete(m_minThreadWorkTime);
	delete(m_maxThreadWorkTime);
	delete(m_diffThreadWorkTime);

	delete(m_avgThreadOverheadTime);
	delete(m_minThreadOverheadTime);
	delete(m_maxThreadOverheadTime);
	delete(m_diffThreadOverheadTime);

	for (auto groupWorkItemStats : *m_workItemStats)
	{
		for (auto workItemStat : *groupWorkItemStats)
			delete(workItemStat);
		delete(groupWorkItemStats);
	}
	delete(m_workItemStats);
}

void ECSLStatistics::AddFrame(ECSLFrame* _frame)
{
	float frameTime = _frame->GetFrameTime();
	m_avgFrameTime = (m_avgFrameTime * m_frameCount + frameTime) / (m_frameCount + 1);
	m_minFrameTime = frameTime < m_minFrameTime ? frameTime : m_minFrameTime;
	m_maxFrameTime = frameTime > m_maxFrameTime ? frameTime : m_maxFrameTime;
	m_diffFrameTime = m_maxFrameTime - m_minFrameTime;

	float totalEfficiency = _frame->GetTotalEfficiency();
	m_avgTotalEfficiency = (m_avgTotalEfficiency * m_frameCount + totalEfficiency) / (m_frameCount + 1);
	m_minTotalEfficiency = totalEfficiency < m_minTotalEfficiency ? totalEfficiency : m_minTotalEfficiency;
	m_maxTotalEfficiency = totalEfficiency > m_maxTotalEfficiency ? totalEfficiency : m_maxTotalEfficiency;
	m_diffTotalEfficiency = m_maxTotalEfficiency - m_minTotalEfficiency;

	for (unsigned int i = 0; i < m_threadCount; ++i)
	{
		float threadEfficiency = _frame->GetThreadEfficiency()[i];
		m_avgThreadEfficiency[i] = (m_avgThreadEfficiency[i] * m_frameCount + threadEfficiency) / (m_frameCount + 1);
		m_minThreadEfficiency[i] = threadEfficiency < m_minThreadEfficiency[i] ? threadEfficiency : m_minThreadEfficiency[i];
		m_maxThreadEfficiency[i] = threadEfficiency > m_maxThreadEfficiency[i] ? threadEfficiency : m_maxThreadEfficiency[i];
		m_diffThreadEfficiency[i] = m_maxThreadEfficiency[i] - m_minThreadEfficiency[i];
	}

	float totalWorkTime = _frame->GetTotalWorkTime();
	m_avgTotalWorkTime = (m_avgTotalWorkTime * m_frameCount + totalWorkTime) / (m_frameCount + 1);
	m_minTotalWorkTime = totalWorkTime < m_minTotalWorkTime ? totalWorkTime : m_minTotalWorkTime;
	m_maxTotalWorkTime = totalWorkTime > m_maxTotalWorkTime ? totalWorkTime : m_maxTotalWorkTime;
	m_diffTotalWorkTime = m_maxTotalWorkTime - m_minTotalWorkTime;

	for (unsigned int i = 0; i < m_threadCount; ++i)
	{
		float threadWorkTime = _frame->GetThreadWorkTime()[i];
		m_avgThreadWorkTime[i] = (m_avgThreadWorkTime[i] * m_frameCount + threadWorkTime) / (m_frameCount + 1);
		m_minThreadWorkTime[i] = threadWorkTime < m_minThreadWorkTime[i] ? threadWorkTime : m_minThreadWorkTime[i];
		m_maxThreadWorkTime[i] = threadWorkTime > m_maxThreadWorkTime[i] ? threadWorkTime : m_maxThreadWorkTime[i];
		m_diffThreadWorkTime[i] = m_maxThreadWorkTime[i] - m_minThreadWorkTime[i];
	}

	float totalOverheadTime = _frame->GetTotalOverheadTime();
	m_avgTotalOverheadTime = (m_avgTotalOverheadTime * m_frameCount + totalOverheadTime) / (m_frameCount + 1);
	m_minTotalOverheadTime = totalOverheadTime < m_minTotalOverheadTime ? totalOverheadTime : m_minTotalOverheadTime;
	m_maxTotalOverheadTime = totalOverheadTime > m_maxTotalOverheadTime ? totalOverheadTime : m_maxTotalOverheadTime;
	m_diffTotalOverheadTime = m_maxTotalOverheadTime - m_minTotalOverheadTime;

	for (unsigned int i = 0; i < m_threadCount; ++i)
	{
		float threadOverheadTime = _frame->GetThreadOverheadTime()[i];
		m_avgThreadOverheadTime[i] = (m_avgThreadOverheadTime[i] * m_frameCount + threadOverheadTime) / (m_frameCount + 1);
		m_minThreadOverheadTime[i] = threadOverheadTime < m_minThreadOverheadTime[i] ? threadOverheadTime : m_minThreadOverheadTime[i];
		m_maxThreadOverheadTime[i] = threadOverheadTime > m_maxThreadOverheadTime[i] ? threadOverheadTime : m_maxThreadOverheadTime[i];
		m_diffThreadOverheadTime[i] = m_maxThreadOverheadTime[i] - m_minThreadOverheadTime[i];
	}

	auto workGroups = _frame->GetWorkItems();

	/* Add work item statistic list */
	if (!m_workItemStats)
	{
		m_workItemStats = new std::vector<std::vector<WorkItemStatistic*>*>(workGroups->size());
		int groupIndex = -1;
		for (auto workGroup : *workGroups)
		{
			(*m_workItemStats)[++groupIndex] = new std::vector<WorkItemStatistic*>(workGroup->size());

			for (unsigned int localIndex = 0; localIndex < workGroup->size(); ++localIndex)
			{
				auto workItem = workGroup->at(localIndex);
				WorkItemStatistic* workItemStat = new WorkItemStatistic();
				workItemStat->name = new std::string(*workItem->name);
				workItemStat->avgDuration = workItem->duration;
				workItemStat->maxDuration = -FLT_MAX;
				workItemStat->minDuration = FLT_MAX;
				(*(*m_workItemStats)[groupIndex])[localIndex] = workItemStat;
			}
		}
	}
	/* Update work item data */
	else
	{
		for (unsigned int groupIndex = 0; groupIndex < workGroups->size(); ++groupIndex)
		{
			auto workGroup = workGroups->at(groupIndex);
			for (unsigned int localIndex = 0; localIndex < workGroup->size(); ++localIndex)
			{
				auto workItem = workGroup->at(localIndex);
				WorkItemStatistic* workItemStat = (*(*m_workItemStats)[groupIndex])[localIndex];
				float duration = workItem->duration;
				workItemStat->avgDuration = (workItemStat->avgDuration * m_frameCount + duration) / (m_frameCount + 1);
				workItemStat->minDuration = duration < workItemStat->minDuration ? duration : workItemStat->minDuration;
				workItemStat->maxDuration = duration > workItemStat->maxDuration ? duration : workItemStat->maxDuration;
				workItemStat->diffDuration = workItemStat->maxDuration - workItemStat->minDuration;
			}
		}
	}


	++m_frameCount;

	delete(_frame);
}