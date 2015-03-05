#include "Game/LuaBridge/AI/LuaDynamicScript.h"

namespace LuaBridge
{
	namespace DynamicScript
	{
		RuleManager rm;
		int index;
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
		}

		int	LoadRuleBook( lua_State* _l )
		{
			std::string filePath = LuaEmbedder::PullString( _l, 1 );

			index = rm.ReadRulebook( filePath.c_str( ) );

			DynamicScripting* ds = DynamicScripting::Instance( );

			rulebook* book = rm.GetRulebook(index);

			ds->SetRuleBook( book );
			
			LuaEmbedder::PushInt( _l, index );
			return 1;
		}

		int	GenerateScriptForLua( lua_State* _l )
		{
			DynamicScripting* ds = DynamicScripting::Instance( );

			ds->GenerateScript( );

			std::vector<Rule> script = ds->GetScript( );

			LuaEmbedder::PushBool( _l, true );
			return 1;
		}

		int	UpdateScriptWeight( lua_State* _l )
		{
			float fitness = LuaEmbedder::PullFloat( _l, 1 );

			DynamicScripting* ds = DynamicScripting::Instance();

			rulebook* book = rm.GetRulebook(0);

			ds->AdjustWeight( fitness );

			if ( index >= 0 )
			{
				rm.StoreRulebook( index );
			}

			LuaEmbedder::PushBool( _l, true );

			return 1;
		}

		int	SetScript( lua_State* _l )
		{
			DSData* dsd = LuaEmbedder::PullObject<DSData>( _l, "DSData", 1 );

			index = LuaEmbedder::PullInt( _l, 2 );

			DynamicScripting* ds = DynamicScripting::Instance( );

			ds->SetScript( dsd->rules );

			return 0;
		}

		int	SetNoOfScriptsToUse(lua_State* _l)
		{
			DynamicScripting* ds = DynamicScripting::Instance();

			ds->SetNumberOfScripts(LuaEmbedder::PullInt(_l, 1));

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
	}
}