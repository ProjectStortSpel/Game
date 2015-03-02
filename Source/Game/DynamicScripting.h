#ifndef DYNAMICSCRIPTING_H
#define DYNAMICSCRIPTING_H

#include <vector>
#include <time.h>
#include <float.h>
#include <iostream>
#include <math.h>
#include <cstdlib>

/* Data struct for the pathfinder to store data */


enum RuleType
{
	humpa_dumpa_Void,
	humpa_dumpa_Stone,
	humpa_dumpa_Players,
	humpa_dumpa_RiverEnd,
	humpa_dumpa_Total
};

struct Rule
{
	RuleType	ruleType;
	float		weight;
	std::string	script;

	bool operator == ( Rule r )
	{
		return ruleType == r.ruleType && weight == r.weight && script == r.script;
	}
	/*
		returns true if the rules have the same type
	*/
	bool operator / ( Rule r )
	{
		return ruleType == r.ruleType;
	}
};

typedef std::vector<Rule> rulebook;

class DynamicScripting
{
public:
	static DynamicScripting*	Instance( );
	static void					Destroy( );
								~DynamicScripting( );

	void						AddRule( Rule _rule );
	void						Sum( );
	void						GenerateScript( );
	void						AdjustWeight( float _fitness );
	float						FitnessFunction( float _fitness );
	void						DistributeLeftOvers( float _leftOver );

	bool						InsertToScript( Rule _rule );
	std::vector<Rule>			GetScript( );
	void						SetScript( std::vector<Rule> _script );
	void						SetRuleBook( std::vector<Rule>* _rules );

	bool						IsInScript( Rule _rule );
	bool						IsTypeInScript( Rule _rule );
	float						RandomFloat( );

private:
								DynamicScripting( );
								DynamicScripting( DynamicScripting const& ) = delete;
	void						operator=( DynamicScripting const& ) = delete;
	static DynamicScripting*	m_instance;


private:
	float						m_totalSum;
	unsigned int				m_noOfScriptsToUse;
	rulebook*					m_ruleBook; //pointer?
	rulebook					m_script;
};

#endif //DYNAMICSCRIPTING_H
