#ifndef CONSOLE_H
#define CONSOLE_H

#include <SDL/SDL.h>

#ifdef WIN32
#ifdef _DEBUG
#include <VLD/vld.h>
#endif
#endif

#include <functional>
#include <vector>
#include <map>
#include <cstring>
#include <queue>
#include <string>


namespace Console
{

#define CONSOLE_DEBUG 1

	enum ArgumentType
	{
		Text,
		Number
	};

	struct DECLSPEC Argument
	{
		ArgumentType ArgType;

		union
		{
			float Number;
			const char* Text;
		};

		Argument(float f) { Number = f; ArgType = ArgumentType::Number; }
		Argument(const char* s) { Text = s; ArgType = ArgumentType::Text; }

		std::string GetString();
	};
		

	typedef std::function<void(std::string, std::vector<Argument>*)> ConsoleHook;

	class DECLSPEC ConsoleManager
	{
	private:
        
        struct Command
        {
            std::string command;
            bool history;
        };
        
		std::map<std::string, ConsoleHook> m_consoleHooks;
		std::vector<std::string> m_history;
		std::vector<std::string> m_commandHistory;
		std::string m_match;
		std::queue<Command> m_commandQueue;

		bool ParseArgs(char* _args, std::vector<Argument>* _vector);

		int m_historyCounter;

		int m_start;
		int m_end;
		bool m_open;


		void ExecuteCommand(const char* _command, bool _history);
	public:

		ConsoleManager();
		~ConsoleManager();
		static ConsoleManager& GetInstance();
		
		void ExecuteCommandQueue();
		void AddToCommandQueue(const char* _command, bool _history);
		void AddToHistory(const char* _command);
		void AddCommand(const char* _name, ConsoleHook _hook);
		void RemoveCommand(const char* _name);
		void ClearCommands();
		void AddMessage(const char* _message);
		void ClearHistory();
		std::vector<std::string> GetHistory(void);
		const char* GetFunctionMatch(const char* _command);
		const char* GetPreviousHistory();
		const char* GetNextHistory();
		const char* GetMatch() { return m_match.c_str(); };

		void ScrollUp();
		void ScrollDown();

		bool IsOpen() { return m_open; }
		void SetOpen(bool _value) { m_open = _value; }
	};
}


#endif