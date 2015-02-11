--  Game Logic Systems
package.path = package.path .. ";../../../Externals/content/scripting/storaspelthreaded/Systems/Server/GameLogicSystems/?.lua"
require "sv_StageSystem"

--	Map Systems
package.path = package.path .. ";../../../Externals/content/scripting/storaspelthreaded/Systems/Server/MapSystems/?.lua"
require "sv_MapGenerator"
--require "sv_CreateMapSystem"
require "sv_RiverSystem"
require "sv_VoidSystem"
require "sv_CheckpointSystem"

--	Light Systems
package.path = package.path .. ";../../../Externals/content/scripting/storaspelthreaded/Systems/Server/LightSystems/?.lua"
require "sv_DirectionalLightSystem"

--	Player Systems
package.path = package.path .. ";../../../Externals/content/scripting/storaspelthreaded/Systems/Server/PlayerSystems/?.lua"
require "sv_PlayerSystem"
require "sv_UnitSystem"

-- AI Systems
package.path = package.path .. ";../../../Externals/content/scripting/storaspelthreaded/Systems/Server/AISystems/?.lua"
require "sv_AICardPickingSystem"
require "sv_AddAISystem"

--	Move Systems
package.path = package.path .. ";../../../Externals/content/scripting/storaspelthreaded/Systems/Server/MoveSystems/?.lua"
require "sv_PostMoveSystem"
require "sv_TestMoveSystem"
require "sv_TestMoveRiverSystem"

--	Card Systems
package.path = package.path .. ";../../../Externals/content/scripting/storaspelthreaded/Systems/Server/CardSystems/?.lua"
require "sv_CreateDeckSystem"
require "sv_CreateCardPickTimer"
require "sv_AddCardPickTimerSystem"
require "sv_SetCardPickTimerSystem"
require "sv_UpdateCardPickTimerSystem"
require "sv_AutoPickCardsSystem"
require "sv_DealCardsSystem"
require "sv_TakeCardStepFromUnitSystem"
require "sv_TakeCardsFromPlayerSystem"
require "sv_PlayCardSystem"

--	Card Action Systems
package.path = package.path .. ";../../../Externals/content/scripting/storaspelthreaded/Systems/Server/CardActionSystems/?.lua"
require "sv_ActionMoveForwardSystem"
require "sv_ActionMoveBackwardSystem"
require "sv_ActionTurnLeftSystem"
require "sv_ActionTurnRightSystem"
require "sv_ActionTurnAroundSystem"

--	Card Ability Systems
package.path = package.path .. ";../../../Externals/content/scripting/storaspelthreaded/Systems/Server/CardAbilitySystems/?.lua"
require "sv_AbilitySprintSystem"
--require "sv_AbilitySlingshotSystem"
require "sv_AbilityIWin"

--	Spawn Systems
package.path = package.path .. ";../../../Externals/content/scripting/storaspelthreaded/Systems/Server/SpawnSystems/?.lua"
--require "sv_SpawnSystem"
require "sv_GiveSpawnLocationSystem"
require "sv_FindSpawnpointSystem"
require "sv_RespawnSystem"
require "sv_PostSpawnSystem"

--	Round Systems
package.path = package.path .. ";../../../Externals/content/scripting/storaspelthreaded/Systems/Server/RoundSystems/?.lua"
require "sv_StepTimerSystem"
require "sv_NewStepSystem"
require "sv_StartNewRoundSystem"
require "sv_PlayCardTimerSystem"

--	Camera Systems
package.path = package.path .. ";../../../Externals/content/scripting/storaspelthreaded/Systems/Server/CameraSystems/?.lua"
require "sv_CameraInterestpointSystem"
require "sv_CameraNewRoundSystem"

--	Misc Systems
package.path = package.path .. ";../../../Externals/content/scripting/storaspelthreaded/Systems/Server/MiscSystems/?.lua"
require "sv_TotemPoleSystem"
require "sv_MoveTotemPieceSystem"
require "sv_GameOverSystem"
require "sv_LogStartSystem"
require "sv_ServerNetworkMessageSystem"
require "sv_HostSettingsSystem"

worldCreator:AddSystemGroup()
worldCreator:AddSystemToCurrentGroup(StageSystem)
worldCreator:AddSystemGroup()
worldCreator:AddSystemToCurrentGroup(MapGenerator)
worldCreator:AddSystemGroup()
worldCreator:AddSystemToCurrentGroup(RiverSystem)
worldCreator:AddSystemGroup()
worldCreator:AddSystemToCurrentGroup(VoidSystem)
worldCreator:AddSystemGroup()
worldCreator:AddSystemToCurrentGroup(CheckpointSystem)
worldCreator:AddSystemGroup()
worldCreator:AddSystemToCurrentGroup(DirectionalLightSystem)
worldCreator:AddSystemGroup()
worldCreator:AddSystemToCurrentGroup(PlayerSystem)
worldCreator:AddSystemGroup()
worldCreator:AddSystemToCurrentGroup(UnitSystem)
worldCreator:AddSystemGroup()
worldCreator:AddSystemToCurrentGroup(AddAISystem)
worldCreator:AddSystemGroup()
worldCreator:AddSystemToCurrentGroup(AICardPickingSystem)
worldCreator:AddSystemGroup()
worldCreator:AddSystemToCurrentGroup(PostMoveSystem)
worldCreator:AddSystemGroup()
--worldCreator:AddSystemGroup()
worldCreator:AddSystemToCurrentGroup(TestMoveSystem)
--worldCreator:AddSystemGroup()
worldCreator:AddSystemToCurrentGroup(TestMoveRiverSystem)

worldCreator:AddSystemGroup()
worldCreator:AddSystemToCurrentGroup(CreateDeckSystem)
worldCreator:AddSystemToCurrentGroup(AddCardPickTimer)
worldCreator:AddSystemToCurrentGroup(SetCardPickTimer)
worldCreator:AddSystemToCurrentGroup(UpdateCardPickTimer)
worldCreator:AddSystemToCurrentGroup(CreateCardPickTimer)
worldCreator:AddSystemToCurrentGroup(AutoPickCards)
worldCreator:AddSystemToCurrentGroup(DealCardsSystem)
worldCreator:AddSystemToCurrentGroup(TakeCardStepsFromUnitSystem)
worldCreator:AddSystemToCurrentGroup(TakeCardsFromPlayerSystem)
worldCreator:AddSystemToCurrentGroup(PlayCardSystem)

worldCreator:AddSystemGroup()
worldCreator:AddSystemToCurrentGroup(ActionMoveForwardSystem)
worldCreator:AddSystemToCurrentGroup(ActionMoveBackwardSystem)
worldCreator:AddSystemToCurrentGroup(ActionTurnLeftSystem)
worldCreator:AddSystemToCurrentGroup(ActionTurnRightSystem)
worldCreator:AddSystemToCurrentGroup(ActionTurnAroundSystem)

worldCreator:AddSystemToCurrentGroup(AbilitySprintSystem)
--worldCreator:AddSystemToCurrentGroup(AbilitySlingshotSystem)
worldCreator:AddSystemToCurrentGroup(AbilityIWin)

worldCreator:AddSystemGroup()

worldCreator:AddSystemToCurrentGroup(GiveSpawnLocationSystem)
worldCreator:AddSystemToCurrentGroup(FindSpawnpointSystem)
worldCreator:AddSystemToCurrentGroup(RespawnSystem)
worldCreator:AddSystemToCurrentGroup(PostSpawnSystem)


worldCreator:AddSystemGroup()
worldCreator:AddSystemToCurrentGroup(StartNewRoundSystem)
worldCreator:AddSystemGroup()
worldCreator:AddSystemToCurrentGroup(StepTimerSystem)
worldCreator:AddSystemGroup()
worldCreator:AddSystemToCurrentGroup(NewStepSystem)
worldCreator:AddSystemGroup()
worldCreator:AddSystemToCurrentGroup(PlayCardTimerSystem)



worldCreator:AddSystemGroup()
worldCreator:AddSystemToCurrentGroup(CameraInterestpointSystem)
worldCreator:AddSystemToCurrentGroup(CameraNewRoundSystem)

worldCreator:AddSystemToCurrentGroup(TotemPoleSystem)
worldCreator:AddSystemToCurrentGroup(MoveTotemPieceSystem)

worldCreator:AddSystemToCurrentGroup(HostSettingsSystem)


worldCreator:AddSystemToCurrentGroup(GameOverSystem)

worldCreator:AddSystemGroup()
worldCreator:AddSystemToCurrentGroup(ServerNetworkMessageSystem)

worldCreator:AddSystemGroup()
worldCreator:AddSystemToCurrentGroup(LogStartSystem)