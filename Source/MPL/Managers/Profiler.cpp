#include "Profiler.h"

using namespace MPL;

Profiler::Profiler()
{

}

Profiler::~Profiler()
{

}

Profiler& Profiler::GetInstance()
{
	static Profiler* instance = new Profiler();
	return *instance;
}