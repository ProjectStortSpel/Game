#include "LuaConsole.h"
#include "LuaEmbedder/LuaEmbedder.h"
#include "Console/Console.h"
#include <algorithm>

namespace LuaBridge
{
  namespace LuaConsole
  {
    //std::map<std::string, lua_State*> CommandLuaStateMap = std::map<std::string, lua_State*>();
    
    int Print(lua_State* L);
	int IsOpen(lua_State* L);
	int ExecuteCommand(lua_State* L);
	int AddToCommandQueue(lua_State* L);
	int AddCommand(lua_State* L);
	int AddToHistory(lua_State* L);

	int RemoveCommand(lua_State* L);
	int ClearCommands(lua_State* L);
	int ClearHistory(lua_State* L);

    void Embed(lua_State* L)
    {
      LuaEmbedder::AddFunction(L, "Print", &Print, "Console");
	  LuaEmbedder::AddFunction(L, "IsOpen", &IsOpen, "Console");

	  LuaEmbedder::AddFunction(L, "ExecuteCommand", &ExecuteCommand, "Console");
	  LuaEmbedder::AddFunction(L, "AddToCommandQueue", &AddToCommandQueue, "Console");
	  LuaEmbedder::AddFunction(L, "AddToHistory", &AddToHistory, "Console");

	  LuaEmbedder::AddFunction(L, "AddCommand", &AddCommand, "Console");

	  LuaEmbedder::AddFunction(L, "RemoveCommand", &RemoveCommand, "Console");
	  LuaEmbedder::AddFunction(L, "ClearCommands", &ClearCommands, "Console");
	  LuaEmbedder::AddFunction(L, "ClearHistory", &ClearHistory, "Console");

    }
    
    int Print(lua_State* L)
    {
      Console::ConsoleManager::GetInstance().AddMessage(LuaEmbedder::PullString(L, 1).c_str());
      return 0;
    }

	int IsOpen(lua_State* L)
	{
		bool open = Console::ConsoleManager::GetInstance().IsOpen();
		LuaEmbedder::PushBool(L, open);

		return 1;
	}

	int ExecuteCommand(lua_State* L)
	{
		Console::ConsoleManager::GetInstance().AddToCommandQueue(LuaEmbedder::PullString(L, 1).c_str(), false);
		return 0;
	}

	int AddToCommandQueue(lua_State* L)
	{
		Console::ConsoleManager::GetInstance().AddToCommandQueue(LuaEmbedder::PullString(L, 1).c_str(), false);
		return 0;
	}

	int AddToHistory(lua_State* L)
	{
		Console::ConsoleManager::GetInstance().AddToHistory(LuaEmbedder::PullString(L, 1).c_str());
		return 0;
	}

	void LuaConsoleFunction(std::string _command, std::vector<Console::Argument>* _args)
	{
        lua_State* L = LuaEmbedder::GetFunctionLuaState(_command);

		if (!L)
			return;

		LuaEmbedder::PushString(L, _command);

		for (int i = 0; i < _args->size(); ++i)
		{
			if (_args->at(i).ArgType == Console::ArgumentType::Text)
				LuaEmbedder::PushString(L, _args->at(i).Text);
			else if (_args->at(i).ArgType == Console::ArgumentType::Number)
				LuaEmbedder::PushFloat(L, _args->at(i).Number);
		}

		LuaEmbedder::CallSavedFunction(_command, _args->size() + 1);
	}

	int AddCommand(lua_State* L)
	{
		std::string command = LuaEmbedder::PullString(L, 1);
		std::transform(command.begin(), command.end(), command.begin(), ::tolower);

		LuaEmbedder::SaveFunction(L, 2, command);
		
		Console::ConsoleManager::GetInstance().AddCommand(command.c_str(), &LuaConsoleFunction);

		return 0;
	}

	int RemoveCommand(lua_State* L)
	{
		Console::ConsoleManager::GetInstance().RemoveCommand(LuaEmbedder::PullString(L, 1).c_str());
		return 0;
	}

	int ClearCommands(lua_State* L)
	{
		Console::ConsoleManager::GetInstance().ClearCommands();
		return 0;
	}

	int ClearHistory(lua_State* L)
	{
		Console::ConsoleManager::GetInstance().ClearHistory();
		return 0;
	}
  }
}