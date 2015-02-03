#ifndef DATALOGGER_H
#define DATALOGGER_H

#include <SDL/SDL.h>
#include <vector>
#include "ECSL/Framework/Components/Tables/ComponentTable.h"
#include "ECSL/Framework/Components/Tables/EntityTable.h"
#include "ECSL/Framework/Systems/SystemWorkGroup.h"

namespace ECSL
{
	class DataLogger
	{
	public:
		enum LogType
		{
			EVERYTHING,
			SYSTEM_ENTITIES,
			COMPONENT_TABLES,
			ENTITY_COMPONENTS
		};

		static DataLogger& GetInstance();
		~DataLogger();

		void SetCurrentWorld(unsigned int _entityCount, EntityTable* _entityTable, const std::vector<ComponentTable*>* _componentTables, const std::vector<SystemWorkGroup*>* _systemWorkGroups);

		/* Will always return false because the function should work with asserts */
		bool WriteToLog(LogType _whatToLog = LogType::EVERYTHING);

	private:
		DataLogger();

		const std::string NewLine = "\r\n";

		unsigned int m_entityCount;
		EntityTable* m_entityTable;
		const std::vector<ComponentTable*>* m_componentTables;
		const std::vector<SystemWorkGroup*>* m_systemWorkGroups;

		void CreateSystemEntitiesMessage(std::string& _out);
		void CreateComponentTablesMessage(std::string& _out);
		void CreateEntityComponents(std::string& _out);
	};
}

#endif