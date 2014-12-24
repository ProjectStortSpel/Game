-- Components
package.path = package.path .. ";../../../Externals/content/scripting/storaspel/components/?.lua"
require "components"
require "mapcomponents"
require "playercomponents"
require "cardcomponents"
require "gamestatecomponents"

require "lightcomponents"


-- Systems
package.path = package.path .. ";../../../Externals/content/scripting/storaspel/systems/?.lua"
require "sh_movementsystem"
require "sh_networkmessagessystem"

if Server then
	require "sv_mapsystem"
	--require "sv_moveplayersystem"
	require "sv_onplayerconnected"
	require "sv_playerssystem"
	require "sv_createspawnpointsystem"
	require "sv_spawnsystem"
	require "sv_lobbysystem"
	require "sv_createdecksystem"
	require "sv_dealcardssystem"
	require "sv_startnewroundsystem"
	require "sv_newstepsystem"
	require "sv_playcardssystem"
	require "sv_riversystem"
	require "sv_moveplayersystem"
	require "sv_steptimersystem"
end


if Client then
--require "cl_pickingphasesystem"
	require "cl_lobbysystem"
	require "cl_cardpositionsystem"
	require "cl_givecardindexsystem"
	require "cl_selectcardsystem"
	require "cl_sortcardindexsystem"
	require "cl_sortselectedcardssystem"
	require "cl_sendselectedcardssystem"
	--require "cl_givecardindexsystem"
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

if Server then
	worldCreator:AddSystemToCurrentGroup(MapSystem)
	--worldCreator:AddSystemToCurrentGroup(PlayerMovementSystem)
	worldCreator:AddSystemToCurrentGroup(OnPlayerConnectedSystem)
	worldCreator:AddSystemToCurrentGroup(PlayersSystem)
	worldCreator:AddSystemToCurrentGroup(CreateSpawnpointSystem)
	worldCreator:AddSystemToCurrentGroup(SpawnSystem)
	worldCreator:AddSystemToCurrentGroup(ServerLobbySystem)
	worldCreator:AddSystemToCurrentGroup(CreateDeckSystem)
	worldCreator:AddSystemToCurrentGroup(DealCardsSystem)
	worldCreator:AddSystemToCurrentGroup(StartNewRoundSystem)
	worldCreator:AddSystemToCurrentGroup(NewStepSystem)
	worldCreator:AddSystemToCurrentGroup(PlayCardsSystem)
	worldCreator:AddSystemToCurrentGroup(RiverSystem)

	worldCreator:AddSystemToCurrentGroup(TurnAroundSystem)
	worldCreator:AddSystemToCurrentGroup(TurnLeftSystem)
	worldCreator:AddSystemToCurrentGroup(TurnRightSystem)
	worldCreator:AddSystemToCurrentGroup(MoveForwardSystem)
	worldCreator:AddSystemToCurrentGroup(MoveBackwardSystem)

	worldCreator:AddSystemToCurrentGroup(StepTimerSystem)

end

if Client then
	worldCreator:AddSystemToCurrentGroup(ClientLobbySystem)
	worldCreator:AddSystemToCurrentGroup(CardPositionSystem)
	worldCreator:AddSystemToCurrentGroup(GiveCardIndexSystem)
	worldCreator:AddSystemToCurrentGroup(SelectCardSystem)
	worldCreator:AddSystemToCurrentGroup(SortCardIndexSystem)
	worldCreator:AddSystemToCurrentGroup(SortSelectedCardSystem)
	worldCreator:AddSystemToCurrentGroup(SendSelectCardSystem)
--worldCreator:AddSystemToCurrentGroup(ClientSendCardSystem)
end

