#ifndef CONSOLE_H
#define CONSOLE_H

#include <SDL/SDL.h>

#ifdef WIN32
#include <VLD/vld.h>
#endif

#include <functional>
#include <vector>
#include <map>
#include <cstring>


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
		

	typedef std::function<void(std::vector<Argument>*)> ConsoleHook;

	class DECLSPEC ConsoleManager
	{
	private:

		std::map<std::string, ConsoleHook> m_consoleHooks;
		std::vector<std::string> m_history;
		std::vector<std::string> m_commandHistory;
		std::string m_match;

		bool ParseArgs(char* _args, std::vector<Argument>* _vector);

		int m_historyCounter;

		int m_start;
		int m_end;

	public:

		ConsoleManager();
		~ConsoleManager();
		static ConsoleManager& GetInstance();

		void ExecuteCommand(const char* _command);
		void AddCommand(const char* _name, ConsoleHook _hook);
		void RemoveCommand(const char* _name);
		void ClearCommands();
		void AddMessage(const char* _message);
		std::vector<std::string> GetHistory(void);
		const char* GetFunctionMatch(const char* _command);
		const char* GetPreviousHistory();
		const char* GetNextHistory();
		const char* GetMatch() { return m_match.c_str(); };

		void ScrollUp();
		void ScrollDown();
	};
}


#endif