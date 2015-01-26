#include "ECSLProfiler.h"

using namespace Profilers;

ECSLProfiler::ECSLProfiler(Renderer::GraphicDevice* _graphics)
: m_graphics(_graphics)
{
	m_active = false;
}

ECSLProfiler::~ECSLProfiler()
{

}

void ECSLProfiler::Toggle()
{
	m_active = m_active ? false : true;
}

void ECSLProfiler::OrganizeData()
{
	MPL::LoggedSession* session = MPL::ThreadLogger::GetInstance().PullSession();
	
	if (!session)
		return;
	
	// The first element in each thread log will always include the 
	// sleep period from the end of last update to the beginning 
	// of this frames' update. TLDR; Unneccessary data.
	for (unsigned int threadId = 0; threadId < session->threadCount; ++threadId)
	{
		auto log = (*session->threadLogs)[threadId];
		log->erase(log->begin());
	}

	ECSLFrame* frame = GenerateFrameData(session);

	delete(session);
}

ECSLFrame* ECSLProfiler::GenerateFrameData(MPL::LoggedSession* _session)
{
	ECSLFrame* frame = new ECSLFrame(_session->threadCount, _session->sessionDuration);
	frame->SetFrameTime(_session->sessionDuration);

	for (unsigned int threadId = 0; threadId < _session->threadLogs->size(); ++threadId)
	{
		auto log = _session->threadLogs->at(threadId);
		for (MPL::LoggedAction* action : *log)
		{
			switch (action->type) 
			{
			case MPL::ActionType::OVERHEAD:
				frame->AddOverheadData(action, threadId);
				break;
			case MPL::ActionType::WORK:
				frame->AddWorkData(action, threadId);
				frame->AddWorkItemStatistic(action, threadId);
				break;
			default:
				break;
			}
		}
	}
	frame->CalculateEfficiencyData(_session->threadCount);

	return frame;
}

void ECSLProfiler::NewFrame()
{

}

void ECSLProfiler::Render()
{
	if (!m_active)
		return;

	RenderAverageEfficiency(0.1f, 0.1f);
}

void ECSLProfiler::RenderAverageEfficiency(float _x, float _y)
{
	int x = 0, y = 0; 
	GetScreenPosition(x, y, _x, _y);
}

void ECSLProfiler::GetScreenPosition(int _outX, int _outY, float _x, float _y)
{
	m_graphics->GetWindowSize(_outX, _outY);
	_outX = _outX * _x;
	_outY = _outY * _y;
}