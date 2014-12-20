#include "LuaConsole.h"
#include "LuaEmbedder/LuaEmbedder.h"
#include "Console/Console.h"
#include <algorithm>

namespace LuaBridge
{
  namespace LuaConsole
  {
    int Print();
	int IsOpen();
	int ExecuteCommand();
	int AddToCommandQueue();
	int AddCommand();

	int RemoveCommand();
	int ClearCommands();
	int ClearHistory();

    void Embed()
    {
      LuaEmbedder::AddFunction("Print", &Print, "Console");
	  LuaEmbedder::AddFunction("IsOpen", &IsOpen, "Console");

	  LuaEmbedder::AddFunction("ExecuteCommand", &ExecuteCommand, "Console");
	  LuaEmbedder::AddFunction("AddToCommandQueue", &AddToCommandQueue, "Console");

	  LuaEmbedder::AddFunction("AddCommand", &AddCommand, "Console");

	  LuaEmbedder::AddFunction("RemoveCommand", &RemoveCommand, "Console");
	  LuaEmbedder::AddFunction("ClearCommands", &ClearCommands, "Console");
	  LuaEmbedder::AddFunction("ClearHistory", &ClearHistory, "Console");

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

	int ExecuteCommand()
	{
		Console::ConsoleManager::GetInstance().ExecuteCommand(LuaEmbedder::PullString(1).c_str());
		return 0;
	}

	int AddToCommandQueue()
	{
		Console::ConsoleManager::GetInstance().AddToCommandQueue(LuaEmbedder::PullString(1).c_str());
		return 0;
	}

	void LuaConsoleFunction(std::string _command, std::vector<Console::Argument>* _args)
	{
		LuaEmbedder::PushString(_command);

		for (int i = 0; i < _args->size(); ++i)
		{
			if (_args->at(i).ArgType == Console::ArgumentType::Text)
				LuaEmbedder::PushString(_args->at(i).Text);
			else if (_args->at(i).ArgType == Console::ArgumentType::Number)
				LuaEmbedder::PushFloat(_args->at(i).Number);
		}

		LuaEmbedder::CallSavedFunction(_command, _args->size() + 1);
	}

	int AddCommand()
	{
		std::string command = LuaEmbedder::PullString(1);
		std::transform(command.begin(), command.end(), command.begin(), ::tolower);

		LuaEmbedder::SaveFunction(2, command);

		Console::ConsoleManager::GetInstance().AddCommand(command.c_str(), &LuaConsoleFunction);

		return 0;
	}

	int RemoveCommand()
	{
		Console::ConsoleManager::GetInstance().RemoveCommand(LuaEmbedder::PullString(1).c_str());
		return 0;
	}

	int ClearCommands()
	{
		Console::ConsoleManager::GetInstance().ClearCommands();
		return 0;
	}

	int ClearHistory()
	{
		Console::ConsoleManager::GetInstance().ClearHistory();
		return 0;
	}
  }
}