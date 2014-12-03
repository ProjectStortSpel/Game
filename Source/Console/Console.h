#ifndef CONSOLE_H
#define CONSOLE_H

#include <SDL/SDL.h>

#ifdef WIN32
#include <VLD/vld.h>
#endif

#include <functional>
#include <vector>
#include <map>


namespace Console
{

#define CONSOLE_DEBUG 0

	struct DECLSPEC cmp_str
	{
		bool operator()(char const *a, char const *b)
		{
			return std::strcmp(a, b) < 0;
		}
	};

	enum ArgumentType
	{
		Text,
		Number
	};

	struct DECLSPEC Argument
	{
		ArgumentType ArgType;

		union Arg
		{
			float Number;
			char* Text;
		} Data;

		Argument(float f) { Data.Number = f; ArgType = ArgumentType::Number; }
		Argument(char* s) { Data.Text = s; ArgType = ArgumentType::Text; }
	};
		

	typedef std::function<void(std::vector<Argument>*)> ConsoleHook;

	class DECLSPEC ConsoleManager
	{
	private:

		std::map<char*, ConsoleHook, cmp_str> m_consoleHooks;

		void ParseArgs(char* _args, std::vector<Argument>* _vector);

	public:

		ConsoleManager();
		~ConsoleManager();

		void ExecuteCommand(char* _command);
		void AddCommand(char* _name, ConsoleHook _hook);

	};
}


#endif