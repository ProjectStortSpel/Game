#ifndef SYSTEMIDMANAGER_H
#define SYSTEMIDMANAGER_H

#include <SDL/SDL.h>
#include <string>
#include <unordered_map>

namespace ECSL
{
	class DECLSPEC SystemIdManager
	{
	public:
		SystemIdManager();
		~SystemIdManager();

		unsigned int GetSystemId(const std::string& _systemName);

	private:

		int m_nextSystemId;
		std::unordered_map<std::string, unsigned int>* m_stringSystemId;
	};
}

#endif