#include "Touch.h"
#include <stdio.h>
using namespace Input;

#ifndef MAX_POINTERS
#   define MAX_POINTERS 10
#endif

struct fingerdata
{
    SDL_FingerID fingerid;
    SDL_bool active;
};

fingerdata fingers[MAX_POINTERS];

int get_sdlfinger_index(SDL_FingerID finger)
{
    int n=0;
    
    // First, try to see if the given ID matches any active one
    for(n=0; n<MAX_POINTERS; n++) {
        if(finger == fingers[n].fingerid) {
            return n;
        }
    }
    
    // ID not found, try to find a free spot
    for(n=0; n<MAX_POINTERS; n++) {
        if(fingers[n].active == SDL_FALSE)
        {
            return n;
        }
    }
    
    // Fail
    return -1;
}

Touch::Touch()
{
}

Touch::~Touch()
{
}

void Touch::Update()
{
	for (std::map<SDL_FingerID, bool>::iterator it = m_thisState.begin(); it != m_thisState.end(); it++)
		m_lastState[it->first] = it->second;
		
	m_pressedState.clear();
	m_releasedState.clear();

	m_deltaPositions.clear();
}

void Touch::PollEvent(SDL_Event e)
{
    char n;
    switch (e.type)
    {
        case SDL_FINGERDOWN:
            n = get_sdlfinger_index(e.tfinger.fingerId);
            
            if (n == -1)
                break;
            
            fingers[n].fingerid = e.tfinger.fingerId;
            fingers[n].active = SDL_TRUE;
			m_pressedState[n]	=	SDL_TRUE;
            m_thisState[n] = true;
            m_positions[n] = FingerPosition(e.tfinger.x, e.tfinger.y);
            break;
            
        case SDL_FINGERUP:
            n = get_sdlfinger_index(e.tfinger.fingerId);
            
            if (n == -1)
                break;
            
            fingers[n].active = SDL_FALSE;
			m_releasedState[n]	=	SDL_TRUE;
	    if (m_thisState[n] == true)
		    m_lastState[n] = true;
            m_thisState[n] = false;
            m_positions[n] = FingerPosition(e.tfinger.x, e.tfinger.y);
            break;
            
        case SDL_FINGERMOTION:
            n = get_sdlfinger_index(e.tfinger.fingerId);
            
            if (n == -1)
                break;
            
            m_positions[n] = FingerPosition(e.tfinger.x, e.tfinger.y);
            m_deltaPositions[n] = FingerPosition(e.tfinger.dx, e.tfinger.dy);
            break;
    }
}

InputState Touch::GetFingerState(SDL_FingerID _finger)
{
	if (m_pressedState.find(_finger) != m_pressedState.end())
		return InputState::PRESSED;

	if (m_releasedState.find(_finger) != m_releasedState.end())
		return InputState::RELEASED;

	bool tLast		=	m_lastState.find(_finger) != m_lastState.end() ? m_lastState[_finger] : false;
	bool tThis		=	m_thisState.find(_finger) != m_thisState.end() ? m_thisState[_finger] : false;

	if (!tThis && tLast)
		return InputState::DOWN;

	return InputState::UP;
}