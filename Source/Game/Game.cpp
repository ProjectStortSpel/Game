#include <SDL/SDL.h>
#include <iostream>
#include "ECSL/ECSL.h"
#include "LuaEmbedder/LuaEmbedder.h"

class TestClass
{
private:
  int m_number;
public:
  TestClass()
  {
    m_number = 2;
    std::cout << "Constructor called" << std::endl;
  }
  ~TestClass()
  {
    std::cout << "Destructor called" << std::endl;
  }

  int Add()
  {
    TestClass* tc = LuaEmbedder::PullObject<TestClass>("TestClass", 1);
    m_number += tc->Number();
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
    //LuaEmbedder::PushInt(m_number);
    return 1;
  }
  
  int SetNumber()
  {
    //m_number = LuaEmbedder::PullInt(1);
    return 0;
  }
};

struct TestClass2
{
  int m_number;
  
  TestClass2()
  {
    m_number = 2;
    std::cout << "Constructor called" << std::endl;
  }
  
  ~TestClass2()
  {
    std::cout << "Destructor called" << std::endl;
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
	SDL_Init(SDL_INIT_EVERYTHING);
	
	LuaEmbedder::Init();
	
	TestClass d;
	
	LuaEmbedder::EmbedClass<TestClass>("TestClass");
	LuaEmbedder::EmbedClassFunction<TestClass>("TestClass", "Add", &TestClass::Add);
	LuaEmbedder::EmbedClassFunction<TestClass>("TestClass", "Do", &TestClass::Do);
	LuaEmbedder::EmbedClassFunction<TestClass>("TestClass", "Print", &TestClass::Print);
	LuaEmbedder::EmbedClassProperty<TestClass>("TestClass", "Number", &TestClass::GetNumber, &TestClass::SetNumber);
	LuaEmbedder::AddObject<TestClass>("TestClass", &d, "e");
	
	LuaEmbedder::EmbedClass<TestClass2>("TestClass2");
	LuaEmbedder::EmbedClassFunction<TestClass2>("TestClass2", "Add", &TestClass2::Add);
	LuaEmbedder::EmbedClassFunction<TestClass2>("TestClass2", "Do", &TestClass2::Do);
	LuaEmbedder::EmbedClassFunction<TestClass2>("TestClass2", "Print", &TestClass2::Print);
	
	LuaEmbedder::EmbedFunction("Func1", &GlobalTestFunction);
	LuaEmbedder::EmbedFunction("Func2", &GlobalTestFunction, "Lib");
	LuaEmbedder::EmbedFunction("Func3", &GlobalTestFunction, "Lib");
	
	LuaEmbedder::EmbedDouble("DeltaTime", 0.1, "Time");
	LuaEmbedder::EmbedInt("Fekke", -12, "Time");
	LuaEmbedder::EmbedUnsignedInt("DeltaTime", 12, "Time");
	LuaEmbedder::EmbedBool("IsParty", true, "Time");
	LuaEmbedder::EmbedString("Where", "Hos Mange", "Time");
	
	LuaEmbedder::Load("test.lua");
	LuaEmbedder::RunFunction("Update");
	LuaEmbedder::RunFunction("Update");
	
	int g = LuaEmbedder::PullInt("g", "Lib");
	std::cout << g << std::endl;
	
	LuaEmbedder::Quit();

	SDL_Quit();
	return 0;
}