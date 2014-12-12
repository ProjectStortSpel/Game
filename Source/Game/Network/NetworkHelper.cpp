#include "NetworkHelper.h"
#include <algorithm>

NetworkHelper::NetworkHelper(ECSL::World** _world)
{
	m_world = _world;
}

NetworkHelper::~NetworkHelper()
{

}

Network::Packet* NetworkHelper::WriteEntityAll(Network::PacketHandler* _ph, unsigned int _e)
{

	std::vector<unsigned int> components;
	(*m_world)->GetEntityComponents(components, _e);

	ECSL::ComponentTypeManager* componentTypeManager = &ECSL::ComponentTypeManager::GetInstance();
	for (short i = components.size() - 1; i >= 0; --i)
	{
		if (!componentTypeManager->GetComponentType(components[i])->GetNetworkSyncState())
		{
			components.erase(components.begin() + i);
		}
	}

	uint64_t id = _ph->StartPack("Entity");

	_ph->WriteInt(id, _e);

	WriteComponents(_ph, id, _e, components);

	return _ph->EndPack(id);
}

Network::Packet* NetworkHelper::WriteEntityDelta(Network::PacketHandler* _ph, unsigned int _e, std::vector<unsigned int>& _componentsChanged)
{
	ECSL::ComponentTypeManager* componentTypeManager = &ECSL::ComponentTypeManager::GetInstance();

	//Get all network synced components
	std::vector<unsigned int> components;
	(*m_world)->GetEntityComponents(components, _e);

	for (short i = components.size() - 1; i >= 0; --i)
	{
		if (!componentTypeManager->GetComponentType(components[i])->GetNetworkSyncState())
		{
			components.erase(components.begin() + i);
		}
	}

	//Get all changed network synced components
	for (short i = _componentsChanged.size() - 1; i >= 0; --i)
	{
		if (!componentTypeManager->GetComponentType(_componentsChanged[i])->GetNetworkSyncState())
		{
			_componentsChanged.erase(_componentsChanged.begin() + i);
		}
	}

	//Get all non changed network synced components
	for (short i = 0; i < _componentsChanged.size(); ++i)
	{
		components.erase(std::remove(components.begin(), components.end(), _componentsChanged[i]), components.end());
	}
	

	uint64_t id = _ph->StartPack("EntityDelta");

	_ph->WriteInt(id, _e);
	
	WriteComponents(_ph, id, _e, components, false);
	WriteComponents(_ph, id, _e, _componentsChanged);

	return _ph->EndPack(id);
}

void NetworkHelper::WriteComponents(Network::PacketHandler* _ph, uint64_t _id, unsigned int _e, std::vector<unsigned int>& _components, bool _data)
{
	ECSL::ComponentTypeManager* componentTypeManager = &ECSL::ComponentTypeManager::GetInstance();
	_ph->WriteInt(_id, _components.size());

	for (short n = 0; n < _components.size(); ++n)
	{
		unsigned int componentId = _components[n];

		ECSL::ComponentType* compType = componentTypeManager->GetComponentType(componentId);

		_ph->WriteString(_id, compType->GetName().c_str());

		if (_data)
		{
			const std::map<unsigned int, ECSL::ComponentDataType>* dataTypes = compType->GetDataTypes();
			for (auto dataType : *dataTypes)
			{
				unsigned int byteOffset = dataType.first;

				_ph->WriteInt(_id, byteOffset);

				char* data = (*m_world)->GetComponent(_e, componentId, byteOffset);

				switch (dataType.second)
				{
				case ECSL::ComponentDataType::BOOL:
					_ph->WriteByte(_id, *data);
					break;
				case ECSL::ComponentDataType::CHAR:
					_ph->WriteString(_id, data);
					break;
				case ECSL::ComponentDataType::INT:
				case ECSL::ComponentDataType::REFERENCE:
					_ph->WriteInt(_id, *(int*)data);
					break;
				case ECSL::ComponentDataType::FLOAT:
					_ph->WriteFloat(_id, *(float*)data);
					break;
				case ECSL::ComponentDataType::MATRIX:
				{
					for (unsigned short j = 0; j < 16; ++j)
					{
						_ph->WriteFloat(_id, ((float*)data)[j]);
					}
					break;
				}
				};
			}
		}
	}
}

void NetworkHelper::ReceiveEntityAll(Network::PacketHandler* _ph, uint64_t _id, Network::NetConnection _nc)
{
	std::vector<unsigned int> components;

	unsigned int e = StartReceiveEntity(_ph, _id, components);

	ReceiveComponents(_ph, _id, e, components);

	EndReceiveEntity(e, components);
}

void NetworkHelper::ReceiveEntityDelta(Network::PacketHandler* _ph, uint64_t _id, Network::NetConnection _nc)
{
	std::vector<unsigned int> components;

	unsigned int e = StartReceiveEntity(_ph, _id, components);

	ReceiveComponents(_ph, _id, e, components, false);
	ReceiveComponents(_ph, _id, e, components);

	EndReceiveEntity(e, components);
}

int	NetworkHelper::StartReceiveEntity(Network::PacketHandler* _ph, uint64_t _id, std::vector<unsigned int>& _components)
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

	(*m_world)->GetEntityComponents(_components, idH);

	ECSL::ComponentTypeManager* componentTypeManager = &ECSL::ComponentTypeManager::GetInstance();
	for (short n = _components.size() - 1; n >= 0; --n)
	{
		if (!componentTypeManager->GetComponentType(_components[n])->GetNetworkSyncState())
		{
			_components.erase(_components.begin() + n);
		}
	}
	return idH;
}

void NetworkHelper::ReceiveComponents(Network::PacketHandler* _ph, uint64_t _id, unsigned int _e, std::vector<unsigned int>& _components, bool _data)
{
	unsigned int num_Comp = _ph->ReadInt(_id);

	ECSL::ComponentTypeManager* componentTypeManager = &ECSL::ComponentTypeManager::GetInstance();

	for (unsigned short i = 0; i < num_Comp; ++i)
	{
		const char* compName = _ph->ReadString(_id);
		unsigned int compType = componentTypeManager->GetTableId(compName);

		//	Check if the entity has the component
		bool hasComponent = false;
		for (short n = _components.size() - 1; n >= 0; --n)
		{
			if (compType == _components[n])
			{
				hasComponent = true;
				_components.erase(_components.begin() + n);
				break;
			}
		}


		if (!hasComponent)
			(*m_world)->CreateComponentAndAddTo(compName, _e);

		if (_data)
		{
			auto dataTypes = componentTypeManager->GetComponentType(compType)->GetDataTypes();

			for (unsigned short j = 0; j < dataTypes->size(); ++j)
			{
				unsigned int offset = _ph->ReadInt(_id);
				char* data = (*m_world)->GetComponent(_e, compType, offset);

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
					memcpy(data, strdata.c_str(), strdata.size() + 1);
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

			ECSL::BitSet::DataType* changedComponents = (ECSL::BitSet::DataType*)(*m_world)->GetComponent(_e, "ChangedComponents", 0);
			changedComponents[bitSetIndex] |= ((ECSL::BitSet::DataType)1) << bitIndex;
		}
	}
}

void NetworkHelper::EndReceiveEntity(unsigned int _e, std::vector<unsigned int>& _components)
{
	//	All entries in _components should be removed
	ECSL::ComponentTypeManager* componentTypeManager = &ECSL::ComponentTypeManager::GetInstance();
	for (unsigned int componentId : _components)
	{
		std::string compName = componentTypeManager->GetComponentType(componentId)->GetName();
		(*m_world)->RemoveComponentFrom(compName, _e);
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