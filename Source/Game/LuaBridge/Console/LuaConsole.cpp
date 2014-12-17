#include "LuaConsole.h"
#include "LuaEmbedder/LuaEmbedder.h"
#include "Console/Console.h"

namespace LuaBridge
{
  namespace LuaConsole
  {
    int Print();
	int StartGame();

    void Embed()
    {
      LuaEmbedder::AddFunction("Print", &Print, "Console");
    }
    
    int Print()
    {
      Console::ConsoleManager::GetInstance().AddMessage(LuaEmbedder::PullString(1).c_str());
      return 0;
    }

	int StartGame()
	{
		return 0;
	}


  }
}