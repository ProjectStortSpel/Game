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
		~SystemIdManager();

		static SystemIdManager& GetInstance();

		template<typename SystemType>
		unsigned int GetSystemId();
		unsigned int GetSystemId(const std::string& _systemName);

	private:
		SystemIdManager();

		int m_nextSystemId;
		std::unordered_map<std::string, unsigned int>* m_stringSystemId;
	};

	template<typename SystemType>
	unsigned int SystemIdManager::GetSystemId()
	{
		static unsigned int id = ++m_nextSystemId;
		return id;
	}
}

#endif