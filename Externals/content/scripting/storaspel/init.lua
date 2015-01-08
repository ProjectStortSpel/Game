-- Components
package.path = package.path .. ";../../../Externals/content/scripting/storaspel/components/?.lua"
require "components"
require "mapcomponents"
require "playercomponents"
require "cardcomponents"


-- Systems
package.path = package.path .. ";../../../Externals/content/scripting/storaspel/systems/?.lua"
require "movementsystem"
require "networkmessagessystem"

if Server then
	require "sv_mapsystem"
	--require "sv_moveplayersystem"
	require "sv_onplayerconnected"
	require "sv_playerssystem"
	require "sv_createspawnpointsystem"
	require "sv_spawnsystem"
	require "sv_lobbysystem"
	require "sv_cardlogicsystem"
end


if Client then
--require "cl_pickingphasesystem"
	require "cl_lobbysystem"
	require "cl_cardpositionsystem"
	require "cl_cardlogicsystem"
end



-- Templates
package.path = package.path .. ";../../../Externals/content/scripting/storaspel/templates/?.lua"
require "map"
require "playertemplates"







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
	worldCreator:AddSystemToCurrentGroup(DealCardSystem)
end

if Client then
	worldCreator:AddSystemToCurrentGroup(ClientLobbySystem)
	worldCreator:AddSystemToCurrentGroup(CardPositionSystem)
	worldCreator:AddSystemToCurrentGroup(SelectCardSystem)
--worldCreator:AddSystemToCurrentGroup(ClientSendCardSystem)
end



GameRunning = false
