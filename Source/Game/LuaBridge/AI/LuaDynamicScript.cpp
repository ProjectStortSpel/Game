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

			LuaEmbedder::PushBool( _l, true );
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
		}
	}
}