#include "ECSLFrame.h"

using namespace Profilers;

ECSLFrame::ECSLFrame(unsigned int _threadCount, float _frameTime)
{
	m_threadCount = _threadCount;
	m_threadEfficiency = FloatArray(_threadCount, 0.0f);
	m_threadOverheadTime = FloatArray(_threadCount, 0.0f);
	m_threadWorkTime = FloatArray(_threadCount, 0.0f);
	m_frameTime = _frameTime;
	m_totalEfficiency = 0.0f;
	m_totalOverheadTime = 0.0f;
	m_totalWorkTime = 0.0f;
	m_workItems = new std::vector<ECSLFrame::WorkItem*>();
}

ECSLFrame::~ECSLFrame()
{
	delete(m_threadEfficiency);
	delete(m_threadWorkTime);
	delete(m_threadOverheadTime);
	for (auto workItem : *m_workItems)
		delete(workItem);
	delete(m_workItems);
}

void ECSLFrame::AddOverheadData(MPL::LoggedAction* _action, unsigned int _threadId)
{
	m_totalOverheadTime += _action->duration;
	m_threadOverheadTime[_threadId] += _action->duration;
}

void ECSLFrame::AddWorkData(MPL::LoggedAction* _action, unsigned int _threadId)
{
	m_totalWorkTime += _action->duration;
	m_threadWorkTime[_threadId] += _action->duration;
}

void ECSLFrame::AddWorkItemStatistic(MPL::LoggedAction* _action, unsigned int _threadId)
{
	ECSLFrame::WorkItem* workItem = new WorkItem();
	workItem->name = new std::string(*_action->workItem->ProfilerName);
	workItem->groupId = _action->workItem->ProfilerGroupId;
	workItem->duration = _action->duration;
	m_workItems->push_back(workItem);
}

void ECSLFrame::CalculateEfficiencyData(unsigned int _threadCount)
{
	m_totalEfficiency = m_totalWorkTime / (m_totalWorkTime + m_totalOverheadTime);
	for (unsigned int threadId = 0; threadId < _threadCount; ++threadId)
		m_threadEfficiency[threadId] = m_threadWorkTime[threadId] != 0.0f ? m_threadWorkTime[threadId] / (m_threadWorkTime[threadId] + m_threadOverheadTime[threadId]) : 0.0f;
}

float* Profilers::FloatArray(unsigned int _size, float _initValue)
{
	float* out = new float[_size];
	for (unsigned int i = 0; i < _size; ++i)
		out[i] = _initValue;
	return out;
}