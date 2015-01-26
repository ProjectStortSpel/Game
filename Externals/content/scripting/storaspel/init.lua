-- Components
package.path = package.path .. ";../../../Externals/content/scripting/storaspel/components/?.lua"
require "components"
require "mapcomponents"
require "playercomponents"
require "cardcomponents"
require "lerpcomponents"
require "gamestatecomponents"

require "lightcomponents"

require "interfacecomponents"
require "buttoncomponents"
require "menucomponents"
require "cameracomponents"

-- Systems
package.path = package.path .. ";../../../Externals/content/scripting/storaspel/systems/?.lua"
require "sh_movementsystem"
require "sh_networkmessagessystem"


-- PICKBOX SYSTEMS
package.path = package.path .. ";../../../Externals/content/scripting/storaspel/systems/pickboxsystems/?.lua"
require "sh_pickboxsystem"
package.path = package.path .. ";../../../Externals/content/scripting/storaspel/systems/?.lua"

require "sh_lerpsystem"
require "sh_moveplayersystem"

if Server then
	require "sv_mapsystem"
	--require "sv_moveplayersystem"
	require "sv_onplayerconnected"
	require "sv_playerssystem"
    require "sv_aisystem"
	require "sv_unitsystem"
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
	require "sv_totempolesystem"
	require "sv_voidsystem"
	require "sv_riversystem"
	require "sv_respawnsystem"
	require "sv_gameoversystem"
	
	require "sv_moveplayersystem"

	require "sv_steptimersystem"
	require "sv_playcardtimersystem"
	
	require "sv_cardpicktimersystem"
	require "sv_autopickcardssystem"
	
	require "sv_takecardsfromplayersystem"
	require "sv_takecardstepsfromunitsystem"
	
	require "sv_directionallightsystem"
	
end



if Client then
	require "cl_lobbysystem"
	--require "cl_selectcardsystem"
	require "cl_playerdonevisualizersystem"
	--require "cl_givecardindexsystem"


	-- CARD SYSTEMS
	package.path = package.path .. ";../../../Externals/content/scripting/storaspel/systems/cardsystems/?.lua"
	require "cl_givecardindexsystem"
	require "cl_sortcardindexsystem"
	require "cl_sortselectedcardssystem"
	require "cl_sendselectedcardssystem"
	
	require "cl_cardaddmodelsystem"
	require "cl_cardhoversystem"
	require "cl_cardpositionsystem"
	require "cl_cardpositionsystem2"
	require "cl_cardselectsystem"
	require "cl_cardprintselectionsystem" -- Debug systems for cards
	package.path = package.path .. ";../../../Externals/content/scripting/storaspel/systems/?.lua"
	
	-- CAMERA SYSTEMS
	package.path = package.path .. ";../../../Externals/content/scripting/storaspel/systems/camerasystems/?.lua"
	--require "cl_cameracreationsystem"
	require "cl_newcamerasystem"
	package.path = package.path .. ";../../../Externals/content/scripting/storaspel/systems/?.lua"
	
	package.path = package.path .. ";../../../Externals/content/scripting/storaspel/systems/interfacesystems/?.lua"
	require "cl_gameinterfacesystem"
	require "cl_hoversizesystem"
	require "cl_gamemenusystem"
	require "cl_optionmenusystem"
	package.path = package.path .. ";../../../Externals/content/scripting/storaspel/systems/?.lua"

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


worldCreator:AddSystemToCurrentGroup(PickBoxSystem)

worldCreator:AddSystemToCurrentGroup(LerpSystem)


worldCreator:AddSystemToCurrentGroup(TrueTestMoveSystem)

if Server then
	worldCreator:AddSystemToCurrentGroup(MapSystem)
	--worldCreator:AddSystemToCurrentGroup(PlayerMovementSystem)


	worldCreator:AddSystemToCurrentGroup(OnPlayerConnectedSystem)
	worldCreator:AddSystemToCurrentGroup(PlayersSystem)
    worldCreator:AddSystemToCurrentGroup(AISystem)
	worldCreator:AddSystemToCurrentGroup(UnitSystem)
	worldCreator:AddSystemToCurrentGroup(GiveSpawnLocation)
	--worldCreator:AddSystemToCurrentGroup(CreateSpawnpointSystem)
	worldCreator:AddSystemToCurrentGroup(SpawnSystem)
	worldCreator:AddSystemToCurrentGroup(ServerLobbySystem)
	worldCreator:AddSystemToCurrentGroup(LogStartSystem)
	
	worldCreator:AddSystemToCurrentGroup(CreateDeckSystem)
	worldCreator:AddSystemToCurrentGroup(DealCardsSystem)
	worldCreator:AddSystemToCurrentGroup(StartNewRoundSystem)
	worldCreator:AddSystemToCurrentGroup(NewStepSystem)
	worldCreator:AddSystemToCurrentGroup(PlayCardSystem)

	worldCreator:AddSystemToCurrentGroup(PostMoveSystem)
	worldCreator:AddSystemToCurrentGroup(FinishSystem)
	worldCreator:AddSystemToCurrentGroup(CheckpointSystem)
	worldCreator:AddSystemToCurrentGroup(TotemPoleSystem)
	worldCreator:AddSystemToCurrentGroup(VoidSystem)
	worldCreator:AddSystemToCurrentGroup(RiverSystem)
	worldCreator:AddSystemToCurrentGroup(RespawnSystem)
	worldCreator:AddSystemToCurrentGroup(TestMoveRiverSystem)
	worldCreator:AddSystemToCurrentGroup(GameOverSystem)

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
	worldCreator:AddSystemToCurrentGroup(DirectionalLightSystem)
	
	--	Timer for picking phase
	worldCreator:AddSystemToCurrentGroup(CreateCardPickTimer)
	worldCreator:AddSystemToCurrentGroup(SetCardPickTimer)
	worldCreator:AddSystemToCurrentGroup(AddCardPickTimer)
	worldCreator:AddSystemToCurrentGroup(UpdateCardPickTimer)
	worldCreator:AddSystemToCurrentGroup(AutoPickCards)
	
end

if Client then
	worldCreator:AddSystemToCurrentGroup(ClientLobbySystem)
	worldCreator:AddSystemToCurrentGroup(GiveCardIndexSystem)
	--worldCreator:AddSystemToCurrentGroup(SelectCardSystem)
	worldCreator:AddSystemToCurrentGroup(SortCardIndexSystem)
	worldCreator:AddSystemToCurrentGroup(SortSelectedCardSystem)
	worldCreator:AddSystemToCurrentGroup(SendSelectCardSystem)
	worldCreator:AddSystemToCurrentGroup(CardPositionSystem)
	worldCreator:AddSystemToCurrentGroup(CardPositionSystem2)
	worldCreator:AddSystemToCurrentGroup(CardAddModelSystem)
	worldCreator:AddSystemToCurrentGroup(CardHoverSystem)
	worldCreator:AddSystemToCurrentGroup(CardSelectSystem)
	worldCreator:AddSystemToCurrentGroup(CardPrintSelectionSystem)
	worldCreator:AddSystemToCurrentGroup(PlayerDoneVisualizer)
	worldCreator:AddSystemToCurrentGroup(PlayerIndicatorSystem)
--worldCreator:AddSystemToCurrentGroup(ClientSendCardSystem)

	worldCreator:AddSystemToCurrentGroup(NewCameraSystem)
	
	worldCreator:AddSystemToCurrentGroup(GameInterfaceSystem)
	worldCreator:AddSystemToCurrentGroup(HoverSizeSystem)
	worldCreator:AddSystemToCurrentGroup(GameMenuSystem)
	worldCreator:AddSystemToCurrentGroup(OptionMenuSystem)
end
