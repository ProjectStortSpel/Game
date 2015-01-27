#include "ECSLProfiler.h"

using namespace Profilers;

ECSLProfiler::ECSLProfiler(Renderer::GraphicDevice* _graphics)
: m_graphics(_graphics)
{
	m_active = false;
	ResetSwapTimer();
	m_threadLogger = &MPL::ThreadLogger::GetInstance();
	m_threadLogger->CreateNewSession();
	
	m_backBufferStatistics = nullptr;
	m_frontBufferStatistics = nullptr;
}

ECSLProfiler::~ECSLProfiler()
{
	if (m_backBufferStatistics)
		delete(m_backBufferStatistics);
	if (m_frontBufferStatistics)
		delete(m_frontBufferStatistics);
}

void ECSLProfiler::Toggle()
{
	if (!m_active)
	{
		m_active = true;
	}
	else
	{
		if (m_frontBufferStatistics)
		{
			delete(m_frontBufferStatistics);
			m_frontBufferStatistics = 0;
		}
		if (m_backBufferStatistics)
		{
			delete(m_backBufferStatistics);
			m_backBufferStatistics = 0;
		}
		ResetSwapTimer();
		m_active = false;
	}
}

void ECSLProfiler::Update(float _dt)
{
	if (!m_active)
	{
		m_threadLogger->CreateNewSession();
		return;
	}

	ECSLFrame* currentFrame = CreateFrame();
	
	if (!currentFrame)
		return;
	
	if (!m_backBufferStatistics)
		m_backBufferStatistics = new ECSLStatistics(currentFrame->GetThreadCount());
	m_backBufferStatistics->AddFrame(currentFrame);

	m_bufferSwapTimer += _dt;
	if (m_bufferSwapTimer >= DefaultBufferSwapTime)
	{
		SwapBuffers();
		ResetSwapTimer();
	}
}

void ECSLProfiler::Render()
{
	if (!m_active)
		return;
	if (!m_frontBufferStatistics)
	{
		RenderWaitingForStats(0.0f, 0.0f);
		return;
	}

	RenderWorkItemStats(0.0f, 0.0f);
}

Profilers::ECSLFrame* ECSLProfiler::CreateFrame()
{
	MPL::LoggedSession* session = m_threadLogger->PullSession();
	if (!session)
		return 0;

	// The first element in each thread log will always include the 
	// sleep period from the end of last update to the beginning 
	// of this frames' update. TLDR; Erasing unneccessary data.
	for (unsigned int threadId = 0; threadId < session->threadCount; ++threadId)
	{
		auto log = (*session->threadLogs)[threadId];
		if (log->size() > 0)
		{
			delete(log->at(0));
			log->erase(log->begin());	
		}
	}

	ECSLFrame* frame = GenerateFrameData(session);
	delete(session);

	m_threadLogger->CreateNewSession();

	return frame;
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

void ECSLProfiler::RenderWaitingForStats(float _x, float _y)
{
	int x = 0, y = 0;
	GetScreenPosition(x, y, _x, _y);

	m_graphics->RenderSimpleText("Waiting for statistics...", x, y);
}

void ECSLProfiler::RenderWorkItemStats(float _x, float _y)
{
	int x = 0, y = 0; 
	GetScreenPosition(x, y, _x, _y);

	auto workItemStats = m_frontBufferStatistics->GetWorkItemStats();
	for (unsigned int groupIndex = 0; groupIndex < workItemStats->size(); ++groupIndex)
	{
		auto workItemGroup = workItemStats->at(groupIndex);
		GetTextBoxPosition(x, y, workItemGroup->size() + 2);

		std::stringstream txtGroup;
		txtGroup << "Group " << groupIndex;
		m_graphics->RenderSimpleText(txtGroup.str(), x, y);
		y += TextHeight;

		for (auto workItemStat : *workItemGroup)
		{
			std::stringstream txtWorkItemStat;
			txtWorkItemStat << *workItemStat.second->name << " " << workItemStat.second->duration;
			m_graphics->RenderSimpleText(txtWorkItemStat.str(), x, y);
			y += TextHeight;
		}

		y += TextHeight;
	}
}

void ECSLProfiler::GetTextBoxPosition(int& _outX, int& _outY, int _lines)
{
	const int maxY = 44;
	const int lineWidth = 52;
	
	/* Reached max size of the screen */
	if (_outY + _lines > maxY)
	{
		_outX += lineWidth;
		_outY = 0;
	}
}

void ECSLProfiler::GetScreenPosition(int& _outX, int& _outY, float _x, float _y)
{
	m_graphics->GetWindowSize(_outX, _outY);
	_outX = (int)(_x * _outX);
	_outY = (int)(_y * _outY);
}

void ECSLProfiler::SwapBuffers()
{
	if (m_frontBufferStatistics)
		delete(m_frontBufferStatistics);
	m_frontBufferStatistics = m_backBufferStatistics;
	m_backBufferStatistics = 0;
}

void ECSLProfiler::ResetSwapTimer()
{
	m_bufferSwapTimer = 0.0f;
}