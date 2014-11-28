#include "System.h"
using namespace ECSL;

void System::AddComponentTypeToFilter(std::string _componentType, ComponentFilter _filter)
{
	switch (_filter)
	{
	case ComponentFilter::Mandatory:
		m_mandatoryComponentTypes.Add(_componentType);
		break;
	case ComponentFilter::RequiresOneOf:
		m_requiresOneOfComponentTypes.Add(_componentType);
		break;
	case ComponentFilter::Excluded:
		m_excludedComponentTypes.Add(_componentType);
		break;

	default:
		printf("Unknown ComponentFilter passed into AddComponentToFilter [System.cpp]\n");
		break;
	}
}

SystemBitmask* System::GetSystemBitmask(ComponentFilter _bitmaskType)
{
	switch (_bitmaskType)
	{
	case ComponentFilter::Mandatory:
		return &m_mandatoryComponentTypes;
		break;
	case ComponentFilter::RequiresOneOf:
		return &m_requiresOneOfComponentTypes;
		break;
	case ComponentFilter::Excluded:
		return &m_excludedComponentTypes;
		break;

	default:
		printf("Unknown ComponentFilter passed into AddComponentToFilter [System.cpp]\n");
		break;
	}
	return 0;
}