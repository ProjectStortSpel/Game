#include "RuleManager.h"

typedef std::ifstream			this_is_a_input_file;
typedef std::ofstream			this_is_a_output_file;
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
			Void 10 450
			*/
			//std::stringstream sstream( this_is_a_line );
			std::stringstream ss;

			ss << this_is_a_line.c_str( );

			std::string buffer;
			std::string this_is_a_string;

			std::getline( ss, buffer, ' ' );
			this_is_a_string = buffer;
			std::getline( ss, buffer, ' ' );
			this_is_a_string += " " + buffer;
			std::getline( ss, buffer );
			float Dynamic_Script_weight = atof( buffer.c_str( ) );

			Rule this_is_a_rule;
			this_is_a_rule.script = this_is_a_string;
			this_is_a_rule.weight = Dynamic_Script_weight;
			this_is_the_rulebook.push_back( this_is_a_rule );
		}
		this->m_ruleBase.insert( std::pair<int, rulebook>( this->count, this_is_the_rulebook ) );
		this->m_fileLocations.insert( std::pair<int, std::string>( this->count, _rulbook_path ) );
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
	rulebook* ret_value = nullptr;

	if ( this->m_ruleBase.find( _index ) != this->m_ruleBase.end( ) )
	{
		ret_value = ( &this->m_ruleBase [_index] );
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
	filelocation::iterator itf = this->m_fileLocations.find( _index );

	if ( it != this->m_ruleBase.end( ) )
	{
		this->m_ruleBase.erase( it );
		this->m_fileLocations.erase( itf );
	}
	else
	{
		printf( "Can't find rulebook at index %i\n", _index );
		return false;
	}
	return true;
}

bool RuleManager::StoreRulebook( int _index )
{
	bool ret_value = false;

	rulebase::iterator itRB = this->m_ruleBase.find( _index );
	if ( itRB != this->m_ruleBase.end( ) )
	{
		rulebook rbook = this->m_ruleBase [_index];

		filelocation::iterator itFL = this->m_fileLocations.find( _index );

		if ( itFL != this->m_fileLocations.end( ) )
		{
			std::string path = this->m_fileLocations [_index];

			this_is_a_output_file file;
			file.open( path );

			if ( file.is_open( ) )
			{
				std::string to_file;

				for ( int i = 0; i < rbook.size( ); i++ )
				{
					to_file = rbook [i].script;
					to_file += " " + std::to_string( rbook [i].weight ) + "\n";

					file << to_file.c_str( );
				}
			}

			file.close( );

		}

	}

	return ret_value;
}