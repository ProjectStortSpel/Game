#include "RuleManager.h"

typedef std::ifstream			this_is_a_input_file;
typedef std::ofstream			this_is_a_output_file;
typedef std::string::size_type	this_is_what_i_found;

RuleManager::RuleManager( )
{
	this->count = 0;
}

int RuleManager::ReadRulebook( const char* _rulebook_path )
{
	int ret_value = -1;

	Filelocations::iterator itfl = m_fileLocations.begin();

	for (; itfl != m_fileLocations.end(); itfl++)
	{
		if (itfl->second == _rulebook_path)
		{
			return itfl->first;
		}
	}

	this_is_a_input_file input_file;
	std::string this_is_a_line;
	input_file.open(_rulebook_path);
	rulebook this_is_the_rulebook;

	if ( input_file.is_open( ) )
	{
		while ( std::getline( input_file, this_is_a_line ) )
		{
			// type weight_PF weight_DS

			/*
			1 Void 10 450
			*/
			//std::stringstream sstream( this_is_a_line );
			std::stringstream ss;

			ss << this_is_a_line.c_str( );

			std::string buffer;
			std::string this_is_a_string;
			std::getline(ss, buffer, ' ');
			int rt = atoi(buffer.c_str());


			std::getline( ss, buffer, ' ' );
			this_is_a_string = buffer;

			while (std::getline(ss, buffer, ' '))
			{
				this_is_a_string += " " + buffer;
			}

			float Dynamic_Script_weight;

			if (std::getline(ss, buffer))
			{
				Dynamic_Script_weight = atof(buffer.c_str());
			}
			else
			{
				printf("Failed loading file in Dynamic Scripting.\n");
				input_file.close();
				return ret_value;
			}
			
			Rule this_is_a_rule;
			this_is_a_rule.ruleType = (RuleType)rt;
			this_is_a_rule.script = this_is_a_string;
			this_is_a_rule.weight = Dynamic_Script_weight;
			this_is_the_rulebook.push_back( this_is_a_rule );
		}
		this->m_library.insert(std::pair<int, rulebook>(this->count, this_is_the_rulebook));
		this->m_fileLocations.insert(std::pair<int, std::string>(this->count, _rulebook_path));
		ret_value = this->count++;

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

	if (this->m_library.find(_index) != this->m_library.end())
	{
		ret_value = (&this->m_library[_index]);
	}
	else
	{
		//printf( "Can't find rulebook at index %i\n", _index );
	}

	return ret_value;
}

bool RuleManager::RemoveRulebook( int _index )
{
	DSLibrary::iterator it = this->m_library.find(_index);
	Filelocations::iterator itf = this->m_fileLocations.find(_index);

	if (it != this->m_library.end())
	{
		this->m_library.erase(it);
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

	DSLibrary::iterator itRB = this->m_library.find(_index);
	if (itRB != this->m_library.end())
	{
		rulebook rbook = this->m_library[_index];

		Filelocations::iterator itFL = this->m_fileLocations.find(_index);

		if ( itFL != this->m_fileLocations.end( ) )
		{
			std::string path = this->m_fileLocations[_index];

			this_is_a_output_file file;
			file.open( path );

			if ( file.is_open( ) )
			{
				std::string to_file;

				for ( int i = 0; i < rbook.size( ); i++ )
				{
					std::stringstream ss;

					ss << (int)(rbook[i].ruleType);
					ss << " ";
					ss << rbook[i].script + " ";
					ss << rbook[i].weight;
					ss << "\n";

					file << ss.str( );
				}
			}

			file.close( );

		}

	}

	return ret_value;
}