#include "LuaConsole.h"
#include "LuaEmbedder/LuaEmbedder.h"
#include "Console/Console.h"

namespace LuaBridge
{
  namespace LuaConsole
  {
    int Print();
	int IsOpen();

    void Embed()
    {
      LuaEmbedder::AddFunction("Print", &Print, "Console");
	  LuaEmbedder::AddFunction("IsOpen", &IsOpen, "Console");
    }
    
    int Print()
    {
      Console::ConsoleManager::GetInstance().AddMessage(LuaEmbedder::PullString(1).c_str());
      return 0;
    }

	int IsOpen()
	{
		bool open = Console::ConsoleManager::GetInstance().IsOpen();
		LuaEmbedder::PushBool(open);

		return 1;
	}


  }
}