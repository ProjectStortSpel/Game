#include <SDL/SDL.h>
#include <iostream>
#include "ECSL/ECSL.h"
#include "LuaEmbedder/LuaEmbedder.h"

class TestClass
{
private:
  int m_number;
public:
  static const char className[];
  static LuaClassHandle<TestClass>::Method methods[];

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
    TestClass* tc = le.GetParameterObject<TestClass>(1);
    TestClass* tc2 = this;
    m_number += tc->GetNumber();
    return 0;
  }
  
  int Do()
  {
    LuaEmbedder& le = LuaEmbedder::GetInstance();
    TestClass* tc = le.GetParameterObject<TestClass>(1);
    le.SetParameterObject<TestClass>(this);
    le.SetParameterObject<TestClass>(tc);
    le.CallMethod<TestClass>("TestFunction", 1);
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
};
 
const char TestClass::className[] = "TestClass";
LuaClassHandle<TestClass>::Method TestClass::methods[] =
{
    METHOD(TestClass, Add),
    METHOD(TestClass, Do),
    METHOD(TestClass, Print),
    {0, 0}
};

int main(int argc, char** argv)
{
	SDL_Init(SDL_INIT_EVERYTHING);
	
	//TestClass d;
	
	LuaEmbedder& le = LuaEmbedder::GetInstance();
	le.EmbedClass<TestClass>();
	//le.AddObject<TestClass>(&d, "d");
	le.Run("test.lua");

	SDL_Quit();
	return 0;
}