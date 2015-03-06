#include "DynamicScripting.h"
#define MAX_NO_OF_TRIES 100
#define MINIMUM_WEIGHT 1
#define MAXIMUM_WEIGHT 100

DynamicScripting* DynamicScripting::m_instance = 0;

DynamicScripting* DynamicScripting::Instance()
{
	if (!DynamicScripting::m_instance)
	{
		DynamicScripting::m_instance = new DynamicScripting();
	}

	return DynamicScripting::m_instance;
}

void DynamicScripting::Destroy()
{
	if (DynamicScripting::m_instance)
		delete DynamicScripting::m_instance;
}

DynamicScripting::DynamicScripting()
{
	srand(time(0));
	m_ruleBook = NULL;
	m_noOfScriptsToUse = 3;
}

DynamicScripting::~DynamicScripting()
{
}

void DynamicScripting::AddRule(Rule _rule)
{
	m_ruleBook->push_back(_rule);
}

void DynamicScripting::Sum()
{
	m_totalSum = 0;

	for (unsigned int i = 0; i < m_ruleBook->size(); i++)
	{
		m_totalSum += (*m_ruleBook)[i].weight;
	}
}

void DynamicScripting::GenerateScript()
{
	m_script.clear();
	unsigned int	noOfTries;
	bool			ruleAddedToScript;

	Sum();

	for (unsigned int i = 0; i < m_noOfScriptsToUse; i++)
	{
		noOfTries			= 0;
		ruleAddedToScript	= false;

		int ruleId, selectedRule;
		float weightSum = 0;
		float randomValue;

		/* Try to add a rule to the script by randomizing a value between 0 and m_totalSum which will find a rule with bigger chance if the weight of the rule is greater.*/
		while (noOfTries < MAX_NO_OF_TRIES && !ruleAddedToScript)
		{
			ruleId = 0;
			weightSum = 0;
			selectedRule = -1;

			randomValue = RandomFloat() * m_totalSum;

			while (selectedRule < 0)
			{
				weightSum += (*m_ruleBook)[ruleId].weight;
				if (weightSum > randomValue)
				{
					selectedRule = ruleId;
				}
				else
				{
					ruleId++;
				}
			}

			ruleAddedToScript = InsertToScript((*m_ruleBook)[selectedRule]);
			noOfTries++;
		}
	}
}

float DynamicScripting::RandomFloat()
{
	return ((float)rand()) / ((float)RAND_MAX);
}

bool DynamicScripting::InsertToScript(Rule _rule)
{
	if (IsTypeInScript(_rule))
	{
		return false;
	}

	m_script.push_back(_rule);
	return true;
}

void DynamicScripting::AdjustWeight(float _fitness)
{
	int noOfActiveScripts = m_script.size();

	/* If we have no active scripts or more than we should have, something is wrong.*/
	if (noOfActiveScripts <= 0 || m_ruleBook->size() < noOfActiveScripts)
	{
		return;
	}

	unsigned int noOfInactiveScripts = m_ruleBook->size() - noOfActiveScripts;
	float adjustment = FitnessFunction(_fitness);
	/* The weight should always be the same so we must adjust the inactive scripts also.*/
	float compensation = -(noOfActiveScripts * adjustment / noOfInactiveScripts);
	float leftOver = 0;

	for (int i = 0; i < m_ruleBook->size(); i++)
	{
		/* If the script is used, add the adjustment.*/
		if (IsInScript((*m_ruleBook)[i]))
		{
			( *m_ruleBook ) [i].weight += adjustment;
		}
		/* Else if it is not used, add the compensation.*/
		else
		{
			( *m_ruleBook ) [i].weight += compensation;
		}

		/* Don't let the weights be set outside the allowed range. Add it to diff to compensate for the adjustments.*/
		if ( ( *m_ruleBook ) [i].weight < MINIMUM_WEIGHT )
		{
			leftOver += ( *m_ruleBook ) [i].weight - MINIMUM_WEIGHT;
			( *m_ruleBook ) [i].weight = MINIMUM_WEIGHT;
		}
		else if ( MAXIMUM_WEIGHT < ( *m_ruleBook ) [i].weight )
		{
			leftOver += ( *m_ruleBook ) [i].weight - MAXIMUM_WEIGHT;
			( *m_ruleBook ) [i].weight = MAXIMUM_WEIGHT;
		}
	}

	DistributeLeftOvers(leftOver);
}

float DynamicScripting::FitnessFunction(float _fitness)
{
	/* Positive outcome. Value between 0.0 - 1.0. */
	if (0.5f <= _fitness && _fitness <= 1.0f)
	{
		float a = _fitness - 0.5f;
		return a * 2;
	}
	/* Negative outcome. Value between -1.0 - 0.0. */
	else if (0.0f <= _fitness && _fitness < 0.5f)
	{
		float a = _fitness - 0.5f;
		return a * 2;
	}

	printf("WRONG FITNESS VALUE, MUST BE BETWEEN 0 - 1\n");
	return _fitness;
}

std::vector<Rule> DynamicScripting::GetScript()
{
	return m_script;
}

void DynamicScripting::SetScript(std::vector<Rule> _script)
{
	m_script = _script;
}

void DynamicScripting::SetRuleBook( rulebook* _rulebook )
{
	this->m_ruleBook = _rulebook;
}

bool DynamicScripting::IsInScript( Rule _rule )
{
	for ( unsigned int i = 0; i < m_script.size( ); i++ )
	{
		if ( _rule == m_script [i] )
		{
			return true;
		}
	}
	return false;
}

bool DynamicScripting::IsTypeInScript( Rule _rule )
{
	for ( unsigned int i = 0; i < m_script.size( ); i++ )
	{
		if ( _rule / m_script [i] )
		{
			return true;
		}
	}
	return false;
}

void DynamicScripting::DistributeLeftOvers(float _leftOver)
{
	unsigned int noOfRulesToChange = 0;
	float leftOver = 0;

	if (_leftOver < 0)
	{
		for (unsigned int i = 0; i < m_ruleBook->size(); i++)
		{
			if ( ( *m_ruleBook ) [i].weight != MAXIMUM_WEIGHT )
			{
				noOfRulesToChange++;
			}
		}

		for (unsigned int i = 0; i < m_ruleBook->size(); i++)
		{
			if ( ( *m_ruleBook ) [i].weight != MAXIMUM_WEIGHT )
			{
				( *m_ruleBook ) [i].weight += _leftOver / noOfRulesToChange;

				if ( MAXIMUM_WEIGHT < ( *m_ruleBook ) [i].weight )
				{
					leftOver += ( *m_ruleBook ) [i].weight - MAXIMUM_WEIGHT;
					( *m_ruleBook ) [i].weight = MAXIMUM_WEIGHT;
				}
			}
		}
	}
	else if (0 < _leftOver)
	{
		for (unsigned int i = 0; i < m_ruleBook->size(); i++)
		{
			if ( ( *m_ruleBook ) [i].weight != MINIMUM_WEIGHT )
			{
				noOfRulesToChange++;
			}
		}

		for (unsigned int i = 0; i < m_ruleBook->size(); i++)
		{
			if ( ( *m_ruleBook ) [i].weight != MINIMUM_WEIGHT )
			{
				( *m_ruleBook ) [i].weight += _leftOver / noOfRulesToChange;

				if ( ( *m_ruleBook ) [i].weight < MINIMUM_WEIGHT )
				{
					leftOver += ( *m_ruleBook ) [i].weight - MINIMUM_WEIGHT;
					( *m_ruleBook ) [i].weight = MINIMUM_WEIGHT;
				}
			}
		}
	}

	if (abs(leftOver) < 0.00001f)
	{
		return;
	}

	/* If we still have leftOvers, recursively call this method.*/
	DistributeLeftOvers(leftOver);
}

void DynamicScripting::SetNumberOfScripts(unsigned int _noOfScriptsToUse)
{
	m_noOfScriptsToUse = _noOfScriptsToUse;
}