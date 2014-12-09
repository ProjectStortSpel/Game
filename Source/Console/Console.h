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
	};
		

	typedef std::function<void(std::vector<Argument>*)> ConsoleHook;

	class DECLSPEC ConsoleManager
	{
	private:

		std::map<std::string, ConsoleHook> m_consoleHooks;

		bool ParseArgs(char* _args, std::vector<Argument>* _vector);

	public:

		ConsoleManager();
		~ConsoleManager();

		void ExecuteCommand(const char* _command);
		void AddCommand(const char* _name, ConsoleHook _hook);
		void RemoveCommand(const char* _name);
		void ClearCommands();

	};
}


#endif