
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
require "sv_AbilitySlingshotSystem"
require "sv_AbilityIWin"

--	Spawn Systems
package.path = package.path .. ";../../../Externals/content/scripting/storaspelthreaded/Systems/Server/SpawnSystems/?.lua"
require "sv_SpawnSystem"
require "sv_GiveSpawnLocationSystem"
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
require "sv_ServerLobbySystem"
require "sv_LogStartSystem"
require "sv_ServerNetworkMessageSystem"

worldCreator:AddSystemGroup()
worldCreator:AddSystemToCurrentGroup(MapGenerator)
worldCreator:AddSystemToCurrentGroup(RiverSystem)
worldCreator:AddSystemToCurrentGroup(VoidSystem)
worldCreator:AddSystemToCurrentGroup(CheckpointSystem)
worldCreator:AddSystemToCurrentGroup(DirectionalLightSystem)
worldCreator:AddSystemToCurrentGroup(PlayerSystem)
worldCreator:AddSystemToCurrentGroup(UnitSystem)
worldCreator:AddSystemToCurrentGroup(AddAISystem)
worldCreator:AddSystemToCurrentGroup(AICardPickingSystem)
worldCreator:AddSystemToCurrentGroup(PostMoveSystem)

worldCreator:AddSystemGroup()
worldCreator:AddSystemToCurrentGroup(TestMoveSystem)
worldCreator:AddSystemGroup()
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
worldCreator:AddSystemGroup()
worldCreator:AddSystemToCurrentGroup(ActionMoveBackwardSystem)
worldCreator:AddSystemGroup()
worldCreator:AddSystemToCurrentGroup(ActionTurnLeftSystem)
worldCreator:AddSystemGroup()
worldCreator:AddSystemToCurrentGroup(ActionTurnRightSystem)
worldCreator:AddSystemGroup()
worldCreator:AddSystemToCurrentGroup(ActionTurnAroundSystem)




worldCreator:AddSystemGroup()
worldCreator:AddSystemToCurrentGroup(AbilitySprintSystem)
worldCreator:AddSystemGroup()
worldCreator:AddSystemToCurrentGroup(AbilitySlingshotSystem)
worldCreator:AddSystemGroup()
worldCreator:AddSystemToCurrentGroup(AbilityIWin)



worldCreator:AddSystemGroup()
worldCreator:AddSystemToCurrentGroup(GiveSpawnLocationSystem)
worldCreator:AddSystemGroup()
worldCreator:AddSystemToCurrentGroup(SpawnSystem)
worldCreator:AddSystemGroup()
worldCreator:AddSystemToCurrentGroup(RespawnSystem)
worldCreator:AddSystemGroup()
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

worldCreator:AddSystemGroup()
worldCreator:AddSystemToCurrentGroup(CameraNewRoundSystem)




worldCreator:AddSystemGroup()
worldCreator:AddSystemToCurrentGroup(TotemPoleSystem)

worldCreator:AddSystemGroup()
worldCreator:AddSystemToCurrentGroup(MoveTotemPieceSystem)

worldCreator:AddSystemGroup()
worldCreator:AddSystemToCurrentGroup(GameOverSystem)

worldCreator:AddSystemGroup()
worldCreator:AddSystemToCurrentGroup(LogStartSystem)

worldCreator:AddSystemGroup()
worldCreator:AddSystemToCurrentGroup(ServerLobbySystem)

worldCreator:AddSystemGroup()
worldCreator:AddSystemToCurrentGroup(ServerNetworkMessageSystem)