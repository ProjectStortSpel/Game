-- Components
package.path = package.path .. ";../../../Externals/content/scripting/storaspel/components/?.lua"
require "components"
require "mapcomponents"
require "playercomponents"


-- Systems
package.path = package.path .. ";../../../Externals/content/scripting/storaspel/systems/?.lua"
require "movementsystem"
require "cardpositionsystem"
require "networkmessagessystem"

if Server then
	require "sv_mapsystem"
	--require "sv_moveplayersystem"
	require "sv_onplayerconnected"
	require "sv_playerssystem"
	require "sv_createspawnpointsystem"
	require "sv_spawnsystem"
end
if Client then
--require "cl_pickingphasesystem"
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
	worldCreator:AddSystemToCurrentGroup(CardPositionSystem)
	--worldCreator:AddSystemToCurrentGroup(PlayerMovementSystem)
	worldCreator:AddSystemToCurrentGroup(OnPlayerConnectedSystem)
	worldCreator:AddSystemToCurrentGroup(PlayersSystem)
	worldCreator:AddSystemToCurrentGroup(CreateSpawnpointSystem)
	worldCreator:AddSystemToCurrentGroup(SpawnSystem)
end

if Client then
--worldCreator:AddSystemToCurrentGroup(ClientSendCardSystem)
end



GameRunning = false
