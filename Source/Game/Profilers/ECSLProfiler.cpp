#include "ECSLProfiler.h"

#include "ECSLGeneralView.h"
#include "ECSLThreadView.h"
#include "ECSLWorkItemView.h"
#include "ECSLLoadingView.h"

using namespace Profilers;

ECSLProfiler::ECSLProfiler(Renderer::GraphicDevice* _graphics)
: m_graphics(_graphics)
{
	m_state = State::INACTIVE;
	m_backBufferStatistics = 0;
	m_frontBufferStatistics = 0;
	ResetSwapTimer();
	m_threadLogger = &MPL::ThreadLogger::GetInstance();

	CreateRenderViews();
}

ECSLProfiler::~ECSLProfiler()
{
	if (m_backBufferStatistics)
		delete(m_backBufferStatistics);
	if (m_frontBufferStatistics)
		delete(m_frontBufferStatistics);
	for (auto renderView : *m_renderViews)
		delete(renderView);
	delete(m_renderViews);
	delete(m_loadingView);
}

void ECSLProfiler::CreateRenderViews()
{
	m_loadingView = new ECSLLoadingView(m_graphics);

	m_renderViews = new std::vector<ECSLRenderView*>();
	m_renderViews->push_back(new ECSLGeneralView(m_graphics));
	m_renderViews->push_back(new ECSLThreadView(m_graphics));
	m_renderViews->push_back(new ECSLWorkItemView(m_graphics));

	m_renderViewIndex = 0;
	m_renderView = (*m_renderViews)[m_renderViewIndex];
}

void ECSLProfiler::Toggle()
{
	if (m_state == State::INACTIVE)
	{
		/* Set the render view to be the "loading screen" render view */
		m_renderView = m_loadingView;
		m_renderView->Update(0);
		m_state = State::COLLECTING_DATA;
	}
	else if (m_state == State::SHOWING_STATISTICS)
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
		m_state = State::INACTIVE;
	}
}

void ECSLProfiler::NextView()
{
	if (m_state != State::SHOWING_STATISTICS)
		return;

	if (!m_renderView->NextSubview(m_frontBufferStatistics))
	{
		if (m_renderViewIndex == (unsigned int)m_renderViews->size() - 1)
		{
			m_renderViewIndex = 0;
			m_renderView = m_renderViews->at(m_renderViewIndex);
			m_renderView->FirstSubview(m_frontBufferStatistics);
			m_renderView->Update(m_frontBufferStatistics);
		}
		else
		{
			++m_renderViewIndex;
			m_renderView = m_renderViews->at(m_renderViewIndex);
			m_renderView->FirstSubview(m_frontBufferStatistics);
			m_renderView->Update(m_frontBufferStatistics);
		}
	}
}

void ECSLProfiler::PreviousView()
{
	if (m_state != State::SHOWING_STATISTICS)
		return;

	if (!m_renderView->PreviousSubview(m_frontBufferStatistics))
	{
		if (m_renderViewIndex == 0)
		{
			m_renderViewIndex = (unsigned int)m_renderViews->size() - 1;
			m_renderView = m_renderViews->at(m_renderViewIndex);
			m_renderView->LastSubview(m_frontBufferStatistics);
			m_renderView->Update(m_frontBufferStatistics);
		}
		else
		{
			--m_renderViewIndex;
			m_renderView = m_renderViews->at(m_renderViewIndex);
			m_renderView->LastSubview(m_frontBufferStatistics);
			m_renderView->Update(m_frontBufferStatistics);
		}
	}
}

void ECSLProfiler::LogDisplayedStatistics()
{
	if (m_state != State::SHOWING_STATISTICS)
		return;
	m_renderView->WriteToLog();
}

void ECSLProfiler::Begin()
{
	if (m_state == State::INACTIVE)
		return;
	m_threadLogger->CreateNewSession();
}

void ECSLProfiler::End()
{
	if (m_state == State::INACTIVE)
		return;
	m_threadLogger->EndSession();
	m_currentFrame = CreateFrame();
}

void ECSLProfiler::Update(float _dt)
{
	switch (m_state)
	{
		case State::INACTIVE:
			return;
		case State::SHOWING_STATISTICS:
			m_renderView = m_renderViews->at(m_renderViewIndex);
			break;
		default:
			break;
	}

	/* Create back buffer if it doesn't exist */
	if (!m_backBufferStatistics)
		m_backBufferStatistics = new ECSLStatistics(m_currentFrame->GetThreadCount());
	/* Add the current frame to the back buffer statistics */
	m_backBufferStatistics->AddFrame(m_currentFrame);

	m_bufferSwapTimer += _dt;
	/* Swap front and back buffer if the swap timer runs out */
	if (m_bufferSwapTimer >= DefaultBufferSwapTime)
	{
		SwapBuffers();
		/* Create the text entries for the render view */
		m_renderView->Update(m_frontBufferStatistics);
		ResetSwapTimer();
	}	
}

void ECSLProfiler::Render()
{
	if (m_state == State::INACTIVE)
		return;

	m_renderView->Display();
}

Profilers::ECSLFrame* ECSLProfiler::CreateFrame()
{
	MPL::LoggedSession* session = m_threadLogger->PullSession();
	if (!session)
		return 0;
	ECSLFrame* frame = GenerateFrameData(session);
	delete(session);
	return frame;
}

ECSLFrame* ECSLProfiler::GenerateFrameData(MPL::LoggedSession* _session)
{
	ECSLFrame* frame = new ECSLFrame(_session->threadCount, _session->sessionDuration);
	frame->SetFrameTime(_session->sessionDuration);

	for (unsigned int threadId = 0; threadId < (unsigned int)_session->threadLogs->size(); ++threadId)
	{
		const auto log = _session->threadLogs->at(threadId);
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

void ECSLProfiler::SwapBuffers()
{
	if (m_frontBufferStatistics)
		delete(m_frontBufferStatistics);
	else
		m_state = State::SHOWING_STATISTICS;

	m_frontBufferStatistics = m_backBufferStatistics;
	m_backBufferStatistics = 0;
}

void ECSLProfiler::ResetSwapTimer()
{
	m_bufferSwapTimer = 0.0f;
}