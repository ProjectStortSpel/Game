#include "Game/LuaBridge/AI/LuaDynamicScript.h"

namespace LuaBridge
{
	namespace DynamicScript
	{
		void Embed( lua_State* _l )
		{
			LuaEmbedder::EmbedClass<DSData>( _l, "DSData" );
			LuaEmbedder::EmbedClassFunction<DSData>( _l, "DSData", "AddElement", &DSData::AddElement );

			LuaEmbedder::AddFunction( _l, "LoadRuleBook", &LoadRuleBook, "DynamicScript" );
			LuaEmbedder::AddFunction( _l, "GenerateScript", &GenerateScriptForLua, "DynamicScript" );
			LuaEmbedder::AddFunction( _l, "UpdateWeight", &UpdateScriptWeight, "DynamicScript" );
			LuaEmbedder::AddFunction( _l, "UseThisScript", &SetScript, "DynamicScript" );
		}

		int	LoadRuleBook( lua_State* _l )
		{
			std::string filePath = LuaEmbedder::PullString( _l, 1 );

			int index = rm.ReadRulebook( filePath.c_str( ) );

			DynamicScripting* ds = DynamicScripting::Instance( );

			ds->SetRuleBook( rm.GetRulebook( index ) );

			LuaEmbedder::PushInt( _l, index );
			return 1;
		}

		int	GenerateScriptForLua( lua_State* _l )
		{
			DynamicScripting* ds = DynamicScripting::Instance( );

			ds->GenerateScript( );

			std::vector<Rule> script = ds->GetScript( );

			LuaEmbedder::PullBool( _l, true );
			return 1;
		}

		int	UpdateScriptWeight( lua_State* _l )
		{
			float fitness = LuaEmbedder::PullFloat( _l, 1 );

			DynamicScripting* ds = DynamicScripting::Instance( );

			ds->AdjustWeight( fitness );

			LuaEmbedder::PushBool( _l, true );
			return 1;
		}

		int	SetScript( lua_State* _l )
		{
			DSData* dsd = LuaEmbedder::PullObject<DSData>( _l, "DSData", 1 );

			DynamicScripting* ds = DynamicScripting::Instance( );

			ds->SetScript( dsd->rules );

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
				LuaEmbedder::PushInt( _l, -1);
			}
			return 1;
		}
	}
}