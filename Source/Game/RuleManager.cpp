#include "RuleManager.h"

typedef std::ifstream			this_is_a_input_file;
typedef std::string::size_type	this_is_what_i_found;

RuleManager::RuleManager( )
{
	this->count = 0;
}

int RuleManager::ReadRulebook( const char* _rulbook_path )
{
	int ret_value = -1;

	this_is_a_input_file input_file;
	std::string this_is_a_line;
	input_file.open( _rulbook_path );
	rulebook this_is_the_rulebook;

	if ( input_file.is_open( ) )
	{
		while ( std::getline( input_file, this_is_a_line ) )
		{
			// type weight_PF weight_DS

			/*
			Void 
			10 
			450 
			*/
			//std::stringstream sstream( this_is_a_line );

			std::string this_is_a_string = this_is_a_line;
			std::getline( input_file, this_is_a_line );
			this_is_a_string += " ";
			this_is_a_string += this_is_a_line;


			std::getline( input_file, this_is_a_line );
			float Dynamic_Script_weight = atof( this_is_a_line.c_str( ) );

			Rule this_is_a_rule;
			this_is_a_rule.script = this_is_a_string;
			this_is_a_rule.weight = Dynamic_Script_weight;
			this_is_the_rulebook.push_back( this_is_a_rule );
		}
		this->m_ruleBase.insert( std::pair<int, rulebook>( count, this_is_the_rulebook ) );
		this->count++;

		input_file.close( );
	}
	else
	{
		//printf( "Cant open file %s\n", _rulbook_path );
	}

	return ret_value;
}

rulebook* RuleManager::GetRulebook( int _index )
{
	rulebook* ret_value;

	if ( this->m_ruleBase.find( _index ) != this->m_ruleBase.end( ) )
	{
		ret_value = (&this->m_ruleBase [_index]);
	}
	else
	{
		//printf( "Can't find rulebook at index %i\n", _index );
	}

	return ret_value;
}

bool RuleManager::RemoveRulebook( int _index )
{
	rulebase::iterator it = this->m_ruleBase.find( _index );

	if ( it != this->m_ruleBase.end( ) )
	{
		this->m_ruleBase.erase( it );
	}
	else
	{
		printf( "Can't find rulebook at index %i\n", _index );
		return false;
	}
	return true;
}