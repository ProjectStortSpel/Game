-- Components
package.path = package.path .. ";../../../Externals/content/scripting/storaspel/components/?.lua"
require "components"
require "mapcomponents"
require "playercomponents"
require "cardcomponents"
require "gamestatecomponents"

require "lightcomponents"

require "interfacecomponents"

-- Systems
package.path = package.path .. ";../../../Externals/content/scripting/storaspel/systems/?.lua"
require "sh_movementsystem"
require "sh_networkmessagessystem"
require "sh_moveplayersystem"

require "sh_pickboxsystem"

if Server then
	require "sv_mapsystem"
	--require "sv_moveplayersystem"
	require "sv_onplayerconnected"
	require "sv_playerssystem"
	require "sv_givespawnlocation"
	--require "sv_createspawnpointsystem"
	require "sv_spawnsystem"
	require "sv_lobbysystem"
	require "sv_createdecksystem"
	require "sv_dealcardssystem"
	require "sv_startnewroundsystem"
	require "sv_newstepsystem"
	require "sv_playcardsystem"

	require "sv_postmovesystem"
	require "sv_checkpointsystem"
	require "sv_finishsystem"
	require "sv_voidsystem"
	require "sv_riversystem"
	require "sv_respawnsystem"

	require "sv_moveplayersystem"

	require "sv_steptimersystem"
	require "sv_playcardtimersystem"

	require "sv_takecardsfromplayersystem"
	require "sv_takecardstepsfromunitsystem"
end


if Client then
--require "cl_pickingphasesystem"
	require "cl_lobbysystem"
	require "cl_cardpositionsystem"
	require "cl_givecardindexsystem"
	--require "cl_selectcardsystem"
	require "cl_sortcardindexsystem"
	require "cl_sortselectedcardssystem"
	require "cl_sendselectedcardssystem"
	require "cl_playerdonevisualizersystem"
	--require "cl_givecardindexsystem"
	require "cl_cardhoversystem"
	require "cl_cardselectsystem"
	require "cl_playerindicatorsystem"
end
	


-- Templates
package.path = package.path .. ";../../../Externals/content/scripting/storaspel/templates/?.lua"
require "map"
require "playertemplates"

require "box"
require "lights"






worldCreator:AddSystemGroup()
worldCreator:AddSystemToCurrentGroup(MovementSystem)
worldCreator:AddSystemToCurrentGroup(networkMessagesSystem)
worldCreator:AddSystemToCurrentGroup(TrueTestMoveSystem)

worldCreator:AddSystemToCurrentGroup(PickBoxSystem)

if Server then
	worldCreator:AddSystemToCurrentGroup(MapSystem)
	--worldCreator:AddSystemToCurrentGroup(PlayerMovementSystem)


	worldCreator:AddSystemToCurrentGroup(OnPlayerConnectedSystem)
	worldCreator:AddSystemToCurrentGroup(PlayersSystem)
	worldCreator:AddSystemToCurrentGroup(GiveSpawnLocation)
	--worldCreator:AddSystemToCurrentGroup(CreateSpawnpointSystem)
	worldCreator:AddSystemToCurrentGroup(SpawnSystem)
	worldCreator:AddSystemToCurrentGroup(ServerLobbySystem)
	worldCreator:AddSystemToCurrentGroup(CreateDeckSystem)
	worldCreator:AddSystemToCurrentGroup(DealCardsSystem)
	worldCreator:AddSystemToCurrentGroup(StartNewRoundSystem)
	worldCreator:AddSystemToCurrentGroup(NewStepSystem)
	worldCreator:AddSystemToCurrentGroup(PlayCardSystem)

	worldCreator:AddSystemToCurrentGroup(PostMoveSystem)
	worldCreator:AddSystemToCurrentGroup(FinishSystem)
	worldCreator:AddSystemToCurrentGroup(CheckpointSystem)
	worldCreator:AddSystemToCurrentGroup(VoidSystem)
	worldCreator:AddSystemToCurrentGroup(RiverSystem)
	worldCreator:AddSystemToCurrentGroup(RespawnSystem)
	worldCreator:AddSystemToCurrentGroup(TestMoveRiverSystem)

	worldCreator:AddSystemToCurrentGroup(TurnAroundSystem)
	worldCreator:AddSystemToCurrentGroup(TurnLeftSystem)
	worldCreator:AddSystemToCurrentGroup(TurnRightSystem)
	worldCreator:AddSystemToCurrentGroup(MoveForwardSystem)
	worldCreator:AddSystemToCurrentGroup(MoveBackwardSystem)
	worldCreator:AddSystemToCurrentGroup(TestMoveSystem)
	
	worldCreator:AddSystemToCurrentGroup(StepTimerSystem)
	worldCreator:AddSystemToCurrentGroup(PlayCardTimerSystem)

	worldCreator:AddSystemToCurrentGroup(TakeCardsFromPlayerSystem)
	worldCreator:AddSystemToCurrentGroup(TakeCardStepsFromUnitSystem)
	
end

if Client then
	worldCreator:AddSystemToCurrentGroup(ClientLobbySystem)
	worldCreator:AddSystemToCurrentGroup(CardPositionSystem)
	worldCreator:AddSystemToCurrentGroup(GiveCardIndexSystem)
	--worldCreator:AddSystemToCurrentGroup(SelectCardSystem)
	worldCreator:AddSystemToCurrentGroup(SortCardIndexSystem)
	worldCreator:AddSystemToCurrentGroup(SortSelectedCardSystem)
	worldCreator:AddSystemToCurrentGroup(SendSelectCardSystem)
	worldCreator:AddSystemToCurrentGroup(CardHoverSystem)
	worldCreator:AddSystemToCurrentGroup(CardSelectSystem)
	worldCreator:AddSystemToCurrentGroup(PlayerDoneVisualizer)
	worldCreator:AddSystemToCurrentGroup(PlayerIndicatorSystem)
--worldCreator:AddSystemToCurrentGroup(ClientSendCardSystem)
end

