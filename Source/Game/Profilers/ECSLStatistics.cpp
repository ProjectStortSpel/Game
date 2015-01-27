#include "ECSLStatistics.h"

using namespace Profilers;

ECSLStatistics::ECSLStatistics(unsigned int _threadCount)
{
	m_threadCount = _threadCount;

	m_avgFrameTime = 0.0f;
	m_minFrameTime = FLT_MAX;
	m_maxFrameTime = -FLT_MAX;;
	m_diffFrameTime = 0.0f;

	m_avgTotalEfficiency = 0.0f;
	m_minTotalEfficiency = FLT_MAX;
	m_maxTotalEfficiency = -FLT_MAX;
	m_diffTotalEfficiency = 0.0f;

	m_avgThreadEfficiency = FloatArray(_threadCount);
	m_minThreadEfficiency = FloatArray(_threadCount);;
	m_maxThreadEfficiency = FloatArray(_threadCount);;
	m_diffThreadEfficiency = FloatArray(_threadCount);;

	m_avgTotalWorkTime = 0.0f;
	m_minTotalWorkTime = FLT_MAX;
	m_maxTotalWorkTime = -FLT_MAX;
	m_diffTotalWorkTime = 0.0f;

	m_avgThreadWorkTime = FloatArray(_threadCount);
	m_minThreadWorkTime = FloatArray(_threadCount);
	m_maxThreadWorkTime = FloatArray(_threadCount);
	m_diffThreadWorkTime = FloatArray(_threadCount);

	m_avgTotalOverheadTime = 0.0f;
	m_minTotalOverheadTime = FLT_MAX;
	m_maxTotalOverheadTime = -FLT_MAX;
	m_diffTotalOverheadTime = 0.0f;

	m_avgThreadOverheadTime = FloatArray(_threadCount);
	m_minThreadOverheadTime = FloatArray(_threadCount);
	m_maxThreadOverheadTime = FloatArray(_threadCount);
	m_diffThreadOverheadTime = FloatArray(_threadCount);

	m_workItemStats = 0;

	m_frames = new std::vector<ECSLFrame*>();
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
		delete(groupWorkItemStats);
	}
	delete(m_workItemStats);

	for (auto frame : *m_frames)
		delete(frame);
	delete(m_frames);
}

void ECSLStatistics::AddFrame(ECSLFrame* _frame)
{
	unsigned int frameCount = (unsigned int)m_frames->size();

	float frameTime = _frame->GetFrameTime();
	m_avgFrameTime = (m_avgFrameTime * frameCount + frameTime) / (frameCount + 1);
	m_minFrameTime = frameTime < m_minFrameTime ? frameTime : m_minFrameTime;
	m_maxFrameTime = frameTime > m_maxFrameTime ? frameTime : m_maxFrameTime;
	m_diffFrameTime = m_maxFrameTime - m_minFrameTime;

	float totalEfficiency = _frame->GetTotalEfficiency();
	m_avgTotalEfficiency = (m_avgTotalEfficiency * frameCount + totalEfficiency) / (frameCount + 1);
	m_minTotalEfficiency = totalEfficiency < m_minTotalEfficiency ? totalEfficiency : m_minTotalEfficiency;
	m_maxTotalEfficiency = totalEfficiency > m_maxTotalEfficiency ? totalEfficiency : m_maxTotalEfficiency;
	m_diffTotalEfficiency = m_maxTotalEfficiency - m_minTotalEfficiency;

	for (unsigned int i = 0; i < m_threadCount; ++i)
	{
		float threadEfficiency = _frame->GetThreadEfficiency()[i];
		m_avgThreadEfficiency[i] = (m_avgThreadEfficiency[i] * frameCount + threadEfficiency) / (frameCount + 1);
		m_minThreadEfficiency[i] = threadEfficiency < m_minThreadEfficiency[i] ? threadEfficiency : m_minThreadEfficiency[i];
		m_maxThreadEfficiency[i] = threadEfficiency > m_maxThreadEfficiency[i] ? threadEfficiency : m_maxThreadEfficiency[i];
		m_diffThreadEfficiency[i] = m_maxThreadEfficiency[i] - m_minThreadEfficiency[i];
	}

	float totalWorkTime = _frame->GetTotalWorkTime();
	m_avgTotalWorkTime = (m_avgTotalWorkTime * frameCount + totalWorkTime) / (frameCount + 1);
	m_minTotalWorkTime = totalWorkTime < m_minTotalWorkTime ? totalWorkTime : m_minTotalWorkTime;
	m_maxTotalWorkTime = totalWorkTime > m_maxTotalWorkTime ? totalWorkTime : m_maxTotalWorkTime;
	m_diffTotalWorkTime = m_maxTotalWorkTime - m_minTotalWorkTime;

	for (unsigned int i = 0; i < m_threadCount; ++i)
	{
		float threadWorkTime = _frame->GetThreadWorkTime()[i];
		m_avgThreadWorkTime[i] = (m_avgThreadWorkTime[i] * frameCount + threadWorkTime) / (frameCount + 1);
		m_minThreadWorkTime[i] = threadWorkTime < m_minThreadWorkTime[i] ? threadWorkTime : m_minThreadWorkTime[i];
		m_maxThreadWorkTime[i] = threadWorkTime > m_maxThreadWorkTime[i] ? threadWorkTime : m_maxThreadWorkTime[i];
		m_diffThreadEfficiency[i] = m_maxThreadWorkTime[i] - m_minThreadWorkTime[i];
	}

	float totalOverheadTime = _frame->GetTotalOverheadTime();
	m_avgTotalOverheadTime = (m_avgTotalOverheadTime * frameCount + totalOverheadTime) / (frameCount + 1);
	m_minTotalOverheadTime = totalOverheadTime < m_minTotalOverheadTime ? totalOverheadTime : m_minTotalOverheadTime;
	m_maxTotalOverheadTime = totalOverheadTime > m_maxTotalOverheadTime ? totalOverheadTime : m_maxTotalOverheadTime;
	m_diffTotalOverheadTime = m_maxTotalOverheadTime - m_minTotalOverheadTime;

	for (unsigned int i = 0; i < m_threadCount; ++i)
	{
		float threadOverheadTime = _frame->GetThreadOverheadTime()[i];
		m_avgThreadOverheadTime[i] = (m_avgThreadOverheadTime[i] * frameCount + threadOverheadTime) / (frameCount + 1);
		m_minThreadOverheadTime[i] = threadOverheadTime < m_minThreadOverheadTime[i] ? threadOverheadTime : m_minThreadOverheadTime[i];
		m_maxThreadOverheadTime[i] = threadOverheadTime > m_maxThreadOverheadTime[i] ? threadOverheadTime : m_maxThreadOverheadTime[i];
		m_diffThreadEfficiency[i] = m_maxThreadOverheadTime[i] - m_minThreadOverheadTime[i];
	}
	
	auto workItemStats = _frame->GetWorkItemStats();

	if (!m_workItemStats)
	{
		m_workItemStats = new std::vector<std::unordered_map<std::string, ECSLFrame::WorkItemStatistic*>*>();
	}

	for (auto workItemStat : *workItemStats)
	{
		while (m_workItemStats->size() <= workItemStat->groupId)
			m_workItemStats->push_back(new std::unordered_map<std::string, ECSLFrame::WorkItemStatistic*>());
		std::unordered_map<std::string, ECSLFrame::WorkItemStatistic*>* groupStats = (*m_workItemStats)[workItemStat->groupId];
		auto it = groupStats->find(*workItemStat->name);
		if (it == groupStats->end())
			(*groupStats)[*workItemStat->name] = workItemStat;
		else
			it->second->duration += workItemStat->duration;
	}

	m_frames->push_back(_frame);
}