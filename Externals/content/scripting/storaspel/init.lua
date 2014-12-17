-- Components
package.path = package.path .. ";../../../Externals/content/scripting/storaspel/components/?.lua"
require "components"

require "playercomponents"


-- Systems
package.path = package.path .. ";../../../Externals/content/scripting/storaspel/systems/?.lua"
--require "mapsystem"
require "movementsystem"
require "networkmessagessystem"

if Server then
	require "sv_onplayerconnected"
	require "sv_playerssystem"
end
if Client then
--require "cl_pickingphasesystem"
end

-- Templates
package.path = package.path .. ";../../../Externals/content/scripting/storaspel/templates/?.lua"
require "playertemplates"

worldCreator:AddSystemGroup()
worldCreator:AddSystemToCurrentGroup(MovementSystem)
worldCreator:AddSystemToCurrentGroup(networkMessagesSystem)

if Server then
	worldCreator:AddSystemToCurrentGroup(OnPlayerConnectedSystem)
	worldCreator:AddSystemToCurrentGroup(PlayersSystem)
end

if Client then
--worldCreator:AddSystemToCurrentGroup(ClientSendCardSystem)
end

