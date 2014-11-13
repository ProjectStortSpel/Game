#include "Test.h"

using namespace ECSL;

Test& Test::GetInstance()
{
	static Test* instance = new Test();
	return *instance;
}