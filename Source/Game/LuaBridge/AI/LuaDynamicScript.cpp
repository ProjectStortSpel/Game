#include "Game/LuaBridge/AI/LuaDynamicScript.h"

namespace LuaBridge
{
	namespace DynamicScript
	{
		RuleManager rm;
		int index;
		DSLibrary scriptLibrary;

		void Embed( lua_State* _l )
		{
			index = -1;
			LuaEmbedder::EmbedClass<DSData>( _l, "DSData" );
			LuaEmbedder::EmbedClassFunction<DSData>( _l, "DSData", "AddElement", &DSData::AddElement );

			LuaEmbedder::AddFunction( _l, "LoadRuleBook", &LoadRuleBook, "DynamicScripting" );
			LuaEmbedder::AddFunction( _l, "GenerateScript", &GenerateScriptForLua, "DynamicScripting" );
			LuaEmbedder::AddFunction( _l, "UpdateWeight", &UpdateScriptWeight, "DynamicScripting" );
			LuaEmbedder::AddFunction( _l, "UseThisScript", &SetScript, "DynamicScripting" );
			LuaEmbedder::AddFunction( _l, "GetRuleTypeInt", &GetRuleTypeInt, "DynamicScripting" );
			LuaEmbedder::AddFunction( _l, "GetWeightFrom", &GetWeightFrom, "DynamicScripting" );
			LuaEmbedder::AddFunction( _l, "SetRuleBook", &SetRuleBook, "DynamicScripting" );
		}

		int	LoadRuleBook( lua_State* _l )
		{
			std::string filePath = LuaEmbedder::PullString( _l, 1 );

			index = rm.ReadRulebook( filePath.c_str( ) );

			LuaEmbedder::PushInt( _l, index );

			return 1;
		}

		int	GenerateScriptForLua( lua_State* _l )
		{
			int playerid = LuaEmbedder::PullInt( _l, 1 );

			DynamicScripting* ds = DynamicScripting::Instance( );

			ds->GenerateScript( );

			std::vector<Rule> script = ds->GetScript( );

			scriptLibrary[playerid] = script;

			LuaEmbedder::PushBool( _l, false );
			return 1;
		}

		int	UpdateScriptWeight( lua_State* _l )
		{
			float fitness = LuaEmbedder::PullFloat( _l, 1 );
			int _book_index = LuaEmbedder::PullInt( _l, 2 );

			rulebook* book = rm.GetRulebook( _book_index );

			DynamicScripting* ds = DynamicScripting::Instance( );

			ds->SetRuleBook( book );
			ds->AdjustWeight( fitness );
			
			if ( _book_index >= 0 )
			{
				rm.StoreRulebook( _book_index );
			}

			LuaEmbedder::PushBool( _l, true );

			return 1;
		}

		int	SetScript( lua_State* _l )
		{
			//DSData* dsd = LuaEmbedder::PullObject<DSData>( _l, "DSData", 1 );

			index = LuaEmbedder::PullInt( _l, 1 );

			DynamicScripting* ds = DynamicScripting::Instance( );

			ds->SetScript( scriptLibrary[index] );

			return 0;
		}

		int	SetNoOfScriptsToUse( lua_State* _l )
		{
			DynamicScripting* ds = DynamicScripting::Instance( );

			ds->SetNumberOfScripts( LuaEmbedder::PullInt( _l, 1 ) );

			return 0;
		}

		int GetRuleTypeInt( lua_State* _l )
		{
			std::string ToInt = LuaEmbedder::PullString( _l, 1 );
			//const char* TrueToInt = ToInt.c_str( );
			if ( ToInt == "Void" )
			{
				LuaEmbedder::PushInt( _l, (int)humpa_dumpa_Void );
			}
			else if ( ToInt == "Stone" )
			{
				LuaEmbedder::PushInt( _l, (int)humpa_dumpa_Stone );
			}
			else if ( ToInt == "River End" )
			{
				LuaEmbedder::PushInt( _l, (int)humpa_dumpa_RiverEnd );
			}
			else if ( ToInt == "Players" )
			{
				LuaEmbedder::PushInt( _l, (int)humpa_dumpa_Players );
			}
			else
			{
				LuaEmbedder::PushInt( _l, -1 );
			}
			return 1;
		}

		int GetWeightFrom( lua_State *_l )
		{
			std::string object = LuaEmbedder::PullString( _l, 1 );
			int playerId = LuaEmbedder::PullInt( _l, 2 );

			DSLibrary::iterator it = scriptLibrary.find( playerId );

			if ( it != scriptLibrary.end( ) )
			{
				
				rulebook rb = scriptLibrary[playerId];
				
				int id = RuleIndex( rb, object );

				if ( id != rb.size( ) )
				{
					LuaEmbedder::PushBool( _l, true );
					std::stringstream ss( rb[id].script );
					std::string this_is_a_string;
					std::getline( ss, this_is_a_string, ' ' );
					int nr_of_stuff = 1;
					//printf( "looking for %s\n", this_is_a_string.c_str( ) );
					while ( std::getline( ss, this_is_a_string, ' ' ) )
					{
						LuaEmbedder::PushFloat( _l, atof( this_is_a_string.c_str( )) );
						//printf( "found this %s\n", this_is_a_string.c_str( ) );
						++nr_of_stuff;
					}

					return nr_of_stuff;
				}
				
			}		
			LuaEmbedder::PushBool( _l, false );
			LuaEmbedder::PushInt( _l, 1 );
			return 2;
		}

		int SetRuleBook( lua_State* _l )
		{
			int _index = LuaEmbedder::PullInt( _l, 1 );

			rulebook* book = rm.GetRulebook( _index );

			DynamicScripting* ds = DynamicScripting::Instance( );

			ds->SetRuleBook( book );

			return 0;
		}

		int	RuleIndex( const rulebook &rb, const std::string &object )
		{
			for ( int i = 0; i < rb.size( ); i++ )
			{
				std::stringstream ss( rb[i].script );
				std::string this_is_a_string;
				std::getline( ss, this_is_a_string, ' ' );

				if ( this_is_a_string == object )
				{
					return i;
				}

				ss.clear( );
			}
			return rb.size( );
		}
	}
}