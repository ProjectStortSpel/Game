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
    LuaEmbedder& le = LuaEmbedder::GetInstance();
    TestClass* tc = le.GetParameterObject<TestClass>("TestClass", 1);
    m_number += tc->Number();
    return 0;
  }
  
  int Do()
  {
    LuaEmbedder& le = LuaEmbedder::GetInstance();
    TestClass* tc = le.GetParameterObject<TestClass>("TestClass", 1);
    le.SetParameterObject<TestClass>("TestClass", tc);
    le.CallMethod<TestClass>("TestClass", "TestFunction", this, 1);
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
    LuaEmbedder& le = LuaEmbedder::GetInstance();
    le.SetParameterInt(m_number);
    return 1;
  }
  
  int SetNumber()
  {
    LuaEmbedder& le = LuaEmbedder::GetInstance();
    m_number = le.GetParameterInt(1);
    return 0;
  }
};

class TestClass2
{
private:
  int m_number;
public:
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
    LuaEmbedder& le = LuaEmbedder::GetInstance();
    TestClass2* tc = le.GetParameterObject<TestClass2>("TestClass2", 1);
    m_number += tc->GetNumber();
    return 0;
  }
  
  int Do()
  {
    LuaEmbedder& le = LuaEmbedder::GetInstance();
    TestClass2* tc = le.GetParameterObject<TestClass2>("TestClass2", 1);
    le.SetParameterObject<TestClass2>("TestClass2", tc);
    le.CallMethod<TestClass2>("TestClass2", "TestFunction", this, 1);
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
    LuaEmbedder& le = LuaEmbedder::GetInstance();
    le.SetParameterInt(5);
    return 1;
  }
};

int main(int argc, char** argv)
{
	SDL_Init(SDL_INIT_EVERYTHING);
	
	//TestClass d;
	
	LuaEmbedder& le = LuaEmbedder::GetInstance();
	le.EmbedClass<TestClass>("TestClass");
	le.EmbedClassFunction<TestClass>("TestClass", "Add", &TestClass::Add);
	le.EmbedClassFunction<TestClass>("TestClass", "Do", &TestClass::Do);
	le.EmbedClassFunction<TestClass>("TestClass", "Print", &TestClass::Print);
	le.EmbedClassProperty<TestClass>("TestClass", "Number", &TestClass::GetNumber, &TestClass::SetNumber);
	//le.AddObject<TestClass>("TestClass", &d, "e");
	
	le.EmbedClass<TestClass2>("TestClass2");
	le.EmbedClassFunction<TestClass2>("TestClass2", "Add", &TestClass2::Add);
	le.EmbedClassFunction<TestClass2>("TestClass2", "Do", &TestClass2::Do);
	le.EmbedClassFunction<TestClass2>("TestClass2", "Print", &TestClass2::Print);
	//le.EmbedClassProperty<TestClass2>("TestClass2", "Number", &TestClass2::Number);
	//le.EmbedClassFunction<TestClass2>("TestClass2", "Number", &TestClass2::Number);
	
	le.Run("test.lua");

	SDL_Quit();
	return 0;
}