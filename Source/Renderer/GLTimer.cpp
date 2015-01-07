#include "GLTimer.h"

#ifndef __ANDROID__
GLTimer::GLTimer()
{
	glGenQueries(1, &startQuery);
	glGenQueries(1, &endQuery);
}

GLTimer::~GLTimer()
{
	glDeleteQueries(1, &startQuery);
	glDeleteQueries(1, &endQuery);
}

void GLTimer::Start()
{
	glQueryCounter(startQuery, GL_TIMESTAMP);
}

float GLTimer::Stop()
{
	glQueryCounter(endQuery, GL_TIMESTAMP);
	stopTimerAvailibale = 0;
	while (!stopTimerAvailibale)
	{
		glGetQueryObjectiv(endQuery, GL_QUERY_RESULT_AVAILABLE, &stopTimerAvailibale);
	}
	glGetQueryObjectui64v(startQuery, GL_QUERY_RESULT, &startTime);
	glGetQueryObjectui64v(endQuery, GL_QUERY_RESULT, &endTime);
	return (endTime - startTime) / 1000000.0;
}
#endif