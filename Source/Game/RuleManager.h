#ifndef RULEMANAGER_H
#define RULEMANAGER_H
#include <map>
#include "Game/DynamicScripting.h"
#include <fstream>
#include <iostream>
#include <string>
#include <stdlib.h>
#include <sstream>

typedef  std::map<int, rulebook> rulebase;
typedef	std::map<int, std::string> filelocation;

class RuleManager
{
private:
	int				count;
	rulebase		m_ruleBase;
	filelocation	m_fileLocations;
public:

	RuleManager( );
	/*
		ReadRulebook:
		Reads the rulebook located at "_rulebook_path".
		Returns the index for the rulebook.
	*/
	int			ReadRulebook( const char* _rulbook_path );
	/*
		GetRuleBook:
		Returns the address to the rulebook that have the "_index" location .
	*/
	rulebook*	GetRulebook( int _index );
	/*
		RemoveRuleBook:
		Removes the rulbook at the "_index" location.
		Returns true if it was able to remove the rulebook.
	*/
	bool		RemoveRulebook( int _index );
	/*
		StoreRulebook:
		Store rulebook at "_index" location, it will be stored in a file.
		Returns true if it was able to store the data.
	*/
	bool		StoreRulebook( int _index );
};

#endif //RULEMANAGER_H
