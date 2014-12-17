GameRunning = false;


-- Components
package.path = package.path .. ";../../../Externals/content/scripting/lobbyTmp/components/?.lua"
require "components"




require "lightcomponents"
require "mapcomponents"
require "playercomponents"
require "cardcomponents"


-- Systems
package.path = package.path .. ";../../../Externals/content/scripting/lobbyTmp/systems/?.lua"
require "systems"

--if Server then
	require "serverlobbysystem"
	require "serverconnectsystem"
--end

--if Client then
	require "clientlobbysystem"
	require "clientconnectsystem"
--end







require "movementsystem"
require "networkmessagessystem"
if Server then
require "mapsystem"
end
require "maplogicsystem"
require "moveplayersystem"
require "networkconnectsystem"
require "cardlogicsystem"
require "discosystem"







-- Templates
package.path = package.path .. ";../../../Externals/content/scripting/lobbyTmp/templates/?.lua"
require "templates"



require "box"
require "lights"
require "map"




worldCreator:AddSystemGroup()
worldCreator:AddSystemToCurrentGroup(StartUpSystem)

--if Server then
	worldCreator:AddSystemToCurrentGroup(ServerLobbySystem)
	worldCreator:AddSystemToCurrentGroup(ServerConnectSystem)
--end

--if Client then
	worldCreator:AddSystemToCurrentGroup(ClientLobbySystem)
	worldCreator:AddSystemToCurrentGroup(ClientConnectSystem)
--end










worldCreator:AddSystemToCurrentGroup(MovementSystem)
worldCreator:AddSystemToCurrentGroup(networkMessagesSystem)
if Server then
worldCreator:AddSystemToCurrentGroup(MapSystem)
end
worldCreator:AddSystemToCurrentGroup(ForwardSystem)
worldCreator:AddSystemToCurrentGroup(BackwardSystem)
worldCreator:AddSystemToCurrentGroup(RightTurnSystem)
worldCreator:AddSystemToCurrentGroup(LeftTurnSystem)
worldCreator:AddSystemToCurrentGroup(TurnAroundSystem)
worldCreator:AddSystemToCurrentGroup(PlayerMovementSystem)
worldCreator:AddSystemToCurrentGroup(RespawnSystem)
worldCreator:AddSystemToCurrentGroup(NewRoundSystem)
worldCreator:AddSystemToCurrentGroup(NetworkConnectSystem)
worldCreator:AddSystemToCurrentGroup(NewStepSystem)
worldCreator:AddSystemToCurrentGroup(WaterMovementSystem)
worldCreator:AddSystemToCurrentGroup(CardDeckSystem)
--worldCreator:AddSystemToCurrentGroup(GetCardsSystem)
worldCreator:AddSystemToCurrentGroup(DiscoSystem)