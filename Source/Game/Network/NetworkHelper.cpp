#include "NetworkHelper.h"


NetworkHelper::NetworkHelper(ECSL::World** _world)
{
	m_world = _world;
}

NetworkHelper::~NetworkHelper()
{

}

Network::Packet* NetworkHelper::WriteEntity(Network::PacketHandler* _ph, unsigned int _e)
{
	
	std::vector<unsigned int> components;
	(*m_world)->GetEntityComponents(components, _e);
	
	ECSL::ComponentTypeManager* componentTypeManager = &ECSL::ComponentTypeManager::GetInstance();
	
	uint64_t id = _ph->StartPack("Entity");
	_ph->WriteInt(id, _e);
	
	for (short i = components.size() - 1; i >= 0; --i)
	{
		if (!componentTypeManager->GetComponentType(components[i])->GetNetworkSyncState())
		{
			components.erase(components.begin() + i);
		}
	}

	_ph->WriteInt(id, components.size());

	for (short n = 0; n < components.size(); ++n)
	{
		unsigned int componentId = components[n];

		ECSL::ComponentType* compType = componentTypeManager->GetComponentType(componentId);

		_ph->WriteString(id, compType->GetName().c_str());

		const std::map<unsigned int, ECSL::ComponentDataType>* dataTypes = compType->GetDataTypes();
		for (auto dataType : *dataTypes)
		{
			unsigned int byteOffset = dataType.first;
			
			_ph->WriteInt(id, byteOffset);

			char* data = (*m_world)->GetComponent(_e, componentId, byteOffset);

			switch (dataType.second)
			{
			case ECSL::ComponentDataType::BOOL:
				_ph->WriteByte(id, *data);
				break;
			case ECSL::ComponentDataType::CHAR:
				_ph->WriteString(id, data);
				break;
			case ECSL::ComponentDataType::INT:
			case ECSL::ComponentDataType::REFERENCE:
				_ph->WriteInt(id, *(int*)data);
				break;
			case ECSL::ComponentDataType::FLOAT:
				_ph->WriteFloat(id, *(float*)data);
				break;
			case ECSL::ComponentDataType::MATRIX:
			{
				for (unsigned short j = 0; j < 16; ++j)
				{
					_ph->WriteFloat(id, ((float*)data)[j]);
				}
				break;
			}
			};
		}

	}
	return _ph->EndPack(id);
}

void NetworkHelper::ReceiveEntity(Network::PacketHandler* _ph, uint64_t _id, Network::NetConnection _nc)
{
	unsigned int idN = _ph->ReadInt(_id);
	unsigned int idH;

	if (m_NtoH.find(idN) != m_NtoH.end())
	{
		idH = m_NtoH[idN];
	}
	else
	{
		idH = (*m_world)->CreateNewEntity();
		printf("Created Entity with ID(NETWORK): %d\n", idH);
		m_NtoH[idN] = idH;
		m_HtoN[idH] = idN;
	}

	unsigned int num_Comp = _ph->ReadInt(_id);

	//	Get the entities components
	std::vector<unsigned int> entityComponents;
	(*m_world)->GetEntityComponents(entityComponents, idH);

	ECSL::ComponentTypeManager* componentTypeManager = &ECSL::ComponentTypeManager::GetInstance();
	for (short n = entityComponents.size() - 1; n >= 0; --n)
	{
		if (!componentTypeManager->GetComponentType(entityComponents[n])->GetNetworkSyncState())
		{
			entityComponents.erase(entityComponents.begin() + n);
		}
	}

	for (unsigned short i = 0; i < num_Comp; ++i)
	{
		const char* compName = _ph->ReadString(_id);
		unsigned int compType = componentTypeManager->GetTableId(compName);

		//	Check if the entity has the component
		bool hasComponent = false;
		for (short n = entityComponents.size() - 1; n >= 0; --n)
		{
			if (compType == entityComponents[n])
			{
				hasComponent = true;
				entityComponents.erase(entityComponents.begin() + n);
				break;
			}
		}
			
		
		if (!hasComponent)
			(*m_world)->CreateComponentAndAddTo(compName, idH);

		auto dataTypes = componentTypeManager->GetComponentType(compType)->GetDataTypes();

		for (unsigned short j = 0; j < dataTypes->size(); ++j)
		{
			unsigned int offset = _ph->ReadInt(_id);
			char* data = (*m_world)->GetComponent(idH, compType, offset);

			ECSL::ComponentDataType compDataType = dataTypes->at(offset);

			switch (compDataType)
			{
			case ECSL::ComponentDataType::BOOL:
			{
				*data = _ph->ReadByte(_id);
			}
			case ECSL::ComponentDataType::CHAR:
			{
				std::string strdata = _ph->ReadString(_id);
				memcpy(data, strdata.c_str(), strdata.size()+1);
				break;
			}
			case ECSL::ComponentDataType::INT:
				*(int*)data = _ph->ReadInt(_id);
				break;
			case ECSL::ComponentDataType::FLOAT:
				*(float*)data = _ph->ReadFloat(_id);
				break;
			case ECSL::ComponentDataType::MATRIX:
			{
				for (unsigned short j = 0; j < 16; ++j)
				{
					((float*)data)[j] = _ph->ReadFloat(_id);
				}
				break;
			}
			case ECSL::ComponentDataType::REFERENCE:
				*(int*)data = m_NtoH[_ph->ReadByte(_id)];
				break;
			};
		}

		int bitSetIndex = ECSL::BitSet::GetBitSetIndex(compType);
		int bitIndex = ECSL::BitSet::GetBitIndex(compType);

		ECSL::BitSet::DataType* changedComponents = (ECSL::BitSet::DataType*)(*m_world)->GetComponent(idH, "ChangedComponents", 0);
		changedComponents[bitSetIndex] |= ((ECSL::BitSet::DataType)1) << bitIndex;
	}

	//	All entries in entityComponents should be removed
	for (unsigned int componentId : entityComponents)
	{
		std::string compName = componentTypeManager->GetComponentType(componentId)->GetName();
		(*m_world)->RemoveComponentFrom(compName, idH);
	}
	
}


void NetworkHelper::ReceiveEntityKill(Network::PacketHandler* _ph, uint64_t _id, Network::NetConnection _nc)
{
	unsigned int idN = _ph->ReadInt(_id);
	
	if (m_NtoH.find(idN) != m_NtoH.end())
	{
		unsigned int idH = m_NtoH[idN];
		(*m_world)->KillEntity(idH);
	}
}