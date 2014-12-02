#ifndef GLTIMER_H
#define GLTIMER_H

#include "stdafx.h"

class GLTimer
{
public:
	GLTimer();
	~GLTimer();
	
	void Start();

	float Stop();

private:

	GLuint startQuery, endQuery;
	GLuint64 startTime, endTime;
	GLint stopTimerAvailibale;
};

#endif