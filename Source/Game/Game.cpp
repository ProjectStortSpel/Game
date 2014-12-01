#include <SDL/SDL.h>
#include <iostream>
#include "ECSL/ECSL.h"
#include "LuaEmbedder/LuaEmbedder.h"

#include "Timer.h"
  
class TestClass
{
private:
  int m_number;
public:
  TestClass()
  {
    m_number = 0;
  }
  ~TestClass()
  {
  }
  
  int Add()
  {
    //TestClass* tc = LuaEmbedder::PullObject<TestClass>("TestClass", 1);
    //m_number += tc->Number();
    m_number += LuaEmbedder::PullDouble(1);
    return 0;
  }
  
  int Do()
  {
    TestClass* tc = LuaEmbedder::PullObject<TestClass>("TestClass", 1);
    LuaEmbedder::PushObject<TestClass>("TestClass", tc);
    LuaEmbedder::CallMethod<TestClass>("TestClass", "TestFunction", this, 1);
    return 0;
  }
  
  int Print()
  {
    std::cout << "Number: " << m_number << std::endl;
    return 0;
  }
  
  int Number()
  {
    return m_number;
  }
  
  int GetNumber()
  {
    LuaEmbedder::PushInt(m_number);
    return 1;
  }
  
  int SetNumber()
  {
    m_number = LuaEmbedder::PullInt(1);
    return 0;
  }
};
  
struct TestClass2
{
  int m_number;
  
  TestClass2()
  {
    m_number = 2;
  }
  
  ~TestClass2()
  {
  }
  
  int Add()
  {
    TestClass2* tc = LuaEmbedder::PullObject<TestClass2>("TestClass2", 1);
    m_number += tc->GetNumber();
    return 0;
  }
  
  int Do()
  {
    TestClass2* tc = LuaEmbedder::PullObject<TestClass2>("TestClass2", 1);
    LuaEmbedder::PushObject<TestClass2>("TestClass2", tc);
    LuaEmbedder::CallMethod<TestClass2>("TestClass2", "TestFunction", this, 1);
    return 0;
  }
  
  int Print()
  {
    std::cout << "Number: " << m_number << std::endl;
    return 0;
  }
  
  int GetNumber()
  {
    return m_number;
  }
  
  int Number()
  {
    //LuaEmbedder::PushInt(5);
    return 1;
  }
};
  
static int GlobalTestFunction()
{
  double a = LuaEmbedder::PullDouble(1);
  double b = LuaEmbedder::PullDouble(2);
  LuaEmbedder::PushDouble(a + b);
  return 1;
}
  
int main(int argc, char** argv)
{
	//SDL_Init(SDL_INIT_EVERYTHING);
  
	Timer t;
	
	LuaEmbedder::Init();
	
	LuaEmbedder::EmbedClass<TestClass>("TestClass");
	LuaEmbedder::EmbedClassFunction<TestClass>("TestClass", "Add", &TestClass::Add);
	LuaEmbedder::EmbedClassFunction<TestClass>("TestClass", "Do", &TestClass::Do);
	LuaEmbedder::EmbedClassFunction<TestClass>("TestClass", "Print", &TestClass::Print);
	LuaEmbedder::EmbedClassFunction<TestClass>("TestClass", "GetNumber", &TestClass::GetNumber);
	LuaEmbedder::EmbedClassProperty<TestClass>("TestClass", "Number", &TestClass::GetNumber, &TestClass::SetNumber);
	
	LuaEmbedder::Load("test.lua");
	
	//LuaEmbedder::CallFunction("Print", 0, "testObject");
	
	//TestClass* pTestObject = LuaEmbedder::PullObject<TestClass>("TestClass", "testObject");
	//LuaEmbedder::CallMethod<TestClass>("TestClass", "Printf", pTestObject);

	t.Reset();
	LuaEmbedder::PushDouble(1);
	LuaEmbedder::PushDouble(2);
	LuaEmbedder::CallMethods<TestClass>("TestClass", "Printf", 2);
	float ms = t.ElapsedTimeInMilliseconds();
	std::cout << "Time elapsed: " << ms << " ms" << std::endl;
	/*
	t.Reset();
	LuaEmbedder::CallFunction("Update");
	std::cout << "Update time: " << t.ElapsedTimeInMilliseconds() << " ms" << std::endl;
	t.Reset();
	LuaEmbedder::CallFunction("Update");
	std::cout << "Update time: " << t.ElapsedTimeInMilliseconds() << " ms" << std::endl;
	t.Reset();
	LuaEmbedder::CallFunction("Update");
	std::cout << "Update time: " << t.ElapsedTimeInMilliseconds() << " ms" << std::endl;
	t.Reset();
	LuaEmbedder::CallFunction("Update");
	std::cout << "Update time: " << t.ElapsedTimeInMilliseconds() << " ms" << std::endl;
	t.Reset();
	LuaEmbedder::CallFunction("Update");
	std::cout << "Update time: " << t.ElapsedTimeInMilliseconds() << " ms" << std::endl;
	*/
	
	
	
	LuaEmbedder::Quit();
  
	//SDL_Quit();
	return 0;
}