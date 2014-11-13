#include "Test.h"

using namespace MPL;

Test& Test::GetInstance()
{
	static Test* instance = new Test();
	return *instance;
}