#ifndef LUADYNAMICSCRIPT_H
#define LUADYNAMICSCRIPT_H

#include <LuaEmbedder/LuaEmbedder.h>
#include "Game/DynamicScripting.h"


namespace LuaBridge
{
	namespace DynamicScript
	{

		struct DSData
		{
			std::vector<Rule> rules;

			DSData( lua_State* _l )
			{

			}

			int AddElement( lua_State* _l )
			{
				float		w = LuaEmbedder::PullFloat( _l, 1 );
				std::string s = LuaEmbedder::PullString( _l, 2 );
				int			t = LuaEmbedder::PullInt( _l, 3 );

				Rule to_add;

				to_add.ruleType = ( RuleType )t;
				to_add.script = s;
				to_add.weight = w;
			}
		};

		void	Embed( lua_State* _l );

		int		LoadRuleBook( lua_State* _l );

		int		GenerateScriptForLua( lua_State* _l );

		int		UpdateScriptWeight( lua_State* _l );

		int		SetScript( lua_State* _l );
	}
}
#endif // LUADYNAMICSCRIPT_H