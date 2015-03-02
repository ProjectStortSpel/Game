--  Game Logic Systems
package.path = package.path .. ";Systems/Server/GameLogicSystems/?.lua"
require "sv_LobbySystem"
require "sv_StageSystem"

--	Map Systems
package.path = package.path .. ";Systems/Server/MapSystems/?.lua"
require "sv_NewMapGenerator"
require "sv_MapRaterSystem"
--require "sv_CreateMapSystem"
require "sv_RiverSystem"
require "sv_VoidSystem"
require "sv_CheckpointSystem"
require "sv_ColorTilesSystem"

--	Light Systems
package.path = package.path .. ";Systems/Server/LightSystems/?.lua"
--require "sv_DirectionalLightSystem"

--	Player Systems
package.path = package.path .. ";Systems/Server/PlayerSystems/?.lua"
require "sv_PlayerSystem"
require "sv_UnitSystem"
require "sv_SendPlayerInformation"

-- AI Systems
package.path = package.path .. ";Systems/Server/AISystems/?.lua"
require "sv_AICardPickingSystem"
require "sv_AddAISystem"

--	Move Systems
package.path = package.path .. ";Systems/Server/MoveSystems/?.lua"
require "sv_PostMoveSystem"
require "sv_TestMoveSystem"
require "sv_TestMoveRiverSystem"
require "sv_SimultaneousMove"

--	Card Systems
package.path = package.path .. ";Systems/Server/CardSystems/?.lua"
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
--require "sv_PlaceCardAboveUnitSystem"
require "sv_SortClientSelectedCards"
require "sv_ClearServerSelectedSystem"

--	Card Action Systems
package.path = package.path .. ";Systems/Server/CardActionSystems/?.lua"
require "sv_ActionMoveForwardSystem"
require "sv_ActionMoveBackwardSystem"
require "sv_ActionTurnLeftSystem"
require "sv_ActionTurnRightSystem"
require "sv_ActionTurnAroundSystem"
require "sv_ActionGuardSystem"

--	Card Ability Systems
package.path = package.path .. ";Systems/Server/CardAbilitySystems/?.lua"
require "sv_AbilitySprintSystem"
require "sv_AbilitySlingshotSystem"
require "sv_AbilityIWin"
require "sv_AbilityStoneSystem"

--	Weather Systems
package.path = package.path .. ";Systems/Server/WeatherSystems/?.lua"
require "sv_WeatherSystem"
require "sv_WeatherWindSystem"
require "sv_WeatherTornadoSystem"

--	Spawn Systems
package.path = package.path .. ";Systems/Server/SpawnSystems/?.lua"
--require "sv_SpawnSystem"
require "sv_GiveSpawnLocationSystem"
require "sv_FindSpawnpointSystem"
require "sv_RespawnSystem"
require "sv_PostSpawnSystem"

--	Round Systems
package.path = package.path .. ";Systems/Server/RoundSystems/?.lua"
require "sv_StepTimerSystem"
require "sv_NewStepSystem"
require "sv_StartNewRoundSystem"
require "sv_PlayCardTimerSystem"

--	Camera Systems
package.path = package.path .. ";Systems/Server/CameraSystems/?.lua"
require "sv_CameraInterestpointSystem"
require "sv_CameraNewRoundSystem"
require "sv_CameraMovePhaseSystem"

--	Misc Systems
package.path = package.path .. ";Systems/Server/MiscSystems/?.lua"
require "sv_TotemPoleSystem"
require "sv_MoveTotemPieceSystem"
require "sv_GameOverSystem"
require "sv_LogStartSystem"
require "sv_ServerNetworkMessageSystem"
require "sv_HostSettingsSystem"
require "sv_OffsetUnitSystem"

--worldCreator:AddSystemGroup()
worldCreator:AddSystemToCurrentGroup(LobbySystem)
worldCreator:AddSystemToCurrentGroup(StageSystem)
worldCreator:AddSystemToCurrentGroup(MapGenerator)
--worldCreator:AddSystemToCurrentGroup(CreateMapSystem)
worldCreator:AddSystemToCurrentGroup(MapRater)
worldCreator:AddSystemToCurrentGroup(RiverSystem)
worldCreator:AddSystemToCurrentGroup(VoidSystem)
worldCreator:AddSystemToCurrentGroup(CheckpointSystem)
worldCreator:AddSystemToCurrentGroup(ColorTilesSystem)
--worldCreator:AddSystemToCurrentGroup(DirectionalLightSystem)
worldCreator:AddSystemToCurrentGroup(PlayerSystem)
worldCreator:AddSystemToCurrentGroup(UnitSystem)
worldCreator:AddSystemToCurrentGroup(OffsetUnitSystem)
worldCreator:AddSystemToCurrentGroup(AddAISystem)
worldCreator:AddSystemToCurrentGroup(AICardPickingSystem)
worldCreator:AddSystemToCurrentGroup(PostMoveSystem)

worldCreator:AddSystemToCurrentGroup(TestMoveSystem)
worldCreator:AddSystemToCurrentGroup(TestMoveRiverSystem)
worldCreator:AddSystemToCurrentGroup(SimultaneousMove)

worldCreator:AddSystemToCurrentGroup(CreateDeckSystem)
worldCreator:AddSystemToCurrentGroup(AddCardPickTimer)
--worldCreator:AddSystemGroup() << TEMP REMOVE 1
worldCreator:AddSystemToCurrentGroup(SetCardPickTimer)
worldCreator:AddSystemToCurrentGroup(UpdateCardPickTimer)
worldCreator:AddSystemToCurrentGroup(CreateCardPickTimer)
worldCreator:AddSystemToCurrentGroup(AutoPickCards)
--worldCreator:AddSystemGroup() << TEMP REMOVE 2
worldCreator:AddSystemToCurrentGroup(DealCardsSystem)
worldCreator:AddSystemToCurrentGroup(TakeCardStepsFromUnitSystem)
worldCreator:AddSystemToCurrentGroup(TakeCardsFromPlayerSystem)
worldCreator:AddSystemToCurrentGroup(PlayCardSystem)
--worldCreator:AddSystemToCurrentGroup(PlaceCardAboveUnitSystem)

--worldCreator:AddSystemGroup() << TEMP REMOVE 3
worldCreator:AddSystemToCurrentGroup(ActionMoveForwardSystem)
worldCreator:AddSystemToCurrentGroup(ActionMoveBackwardSystem)
worldCreator:AddSystemToCurrentGroup(ActionTurnLeftSystem)
worldCreator:AddSystemToCurrentGroup(ActionTurnRightSystem)
worldCreator:AddSystemToCurrentGroup(ActionTurnAroundSystem)
worldCreator:AddSystemToCurrentGroup(ActionGuardSystem)

worldCreator:AddSystemToCurrentGroup(AbilitySprintSystem)
worldCreator:AddSystemToCurrentGroup(AbilitySlingshotSystem)
worldCreator:AddSystemToCurrentGroup(AbilityIWin)
worldCreator:AddSystemToCurrentGroup(AbilityStoneSystem)

worldCreator:AddSystemToCurrentGroup(WeatherSystem)
worldCreator:AddSystemToCurrentGroup(WeatherWindSystem)
worldCreator:AddSystemToCurrentGroup(WeatherTornadoSystem)

worldCreator:AddSystemToCurrentGroup(GiveSpawnLocationSystem)

worldCreator:AddSystemToCurrentGroup(RespawnSystem)
worldCreator:AddSystemToCurrentGroup(PostSpawnSystem)


worldCreator:AddSystemToCurrentGroup(StartNewRoundSystem)
worldCreator:AddSystemToCurrentGroup(StepTimerSystem)
worldCreator:AddSystemToCurrentGroup(NewStepSystem)
worldCreator:AddSystemToCurrentGroup(PlayCardTimerSystem)



--worldCreator:AddSystemGroup() << TEMP 0
worldCreator:AddSystemToCurrentGroup(CameraInterestpointSystem)
worldCreator:AddSystemToCurrentGroup(CameraNewRoundSystem)
worldCreator:AddSystemToCurrentGroup(CameraMovePhaseSystem)

worldCreator:AddSystemToCurrentGroup(TotemPoleSystem)
worldCreator:AddSystemToCurrentGroup(MoveTotemPieceSystem)

worldCreator:AddSystemToCurrentGroup(HostSettingsSystem)


worldCreator:AddSystemToCurrentGroup(GameOverSystem)

worldCreator:AddSystemGroup()
worldCreator:AddSystemToCurrentGroup(ServerNetworkMessageSystem)
worldCreator:AddSystemToCurrentGroup(LogStartSystem)



worldCreator:AddSystemToCurrentGroup(SendPlayerInformation)	--	ALSO TEMP
worldCreator:AddSystemToCurrentGroup(SortClientSelectedCards)	--	Temp
worldCreator:AddSystemToCurrentGroup(ClearServerSelectedSystem)	--	Temp
worldCreator:AddSystemToCurrentGroup(FindSpawnpointSystem)

--	Audio Systems
package.path = package.path .. ";Systems/Server/AudioSystems/?.lua"
require "sv_AudioUnitDeathSystem"

worldCreator:AddSystemGroup()
worldCreator:AddSystemToCurrentGroup(AudioUnitDeathSystem)
