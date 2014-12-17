#include "MathHelper.h"

using namespace MPL;

void MathHelper::SplitIterations(unsigned int& _startAt, unsigned int& _endAt, unsigned int _iterations, unsigned int _taskIndex, unsigned int _taskCount)
{
	unsigned int remainder = _iterations % _taskCount;
	unsigned int evenlySplit = (_iterations - remainder) / _taskCount;
	unsigned int toDo = evenlySplit;
	if (_taskIndex < remainder)
	{
		++toDo;
		_startAt = (evenlySplit * _taskIndex) + (_taskIndex % remainder);
	}
	else
		_startAt = (evenlySplit * _taskIndex) + remainder;
	_endAt = _startAt + toDo;
}