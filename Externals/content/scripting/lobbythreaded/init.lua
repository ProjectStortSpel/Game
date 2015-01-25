GameRunning = false;


-- Components
package.path = package.path .. ";../../../Externals/content/scripting/lobbythreaded/components/?.lua"
require "components"
require "interfacecomponents"
require "buttoncomponents"

-- Systems
package.path = package.path .. ";../../../Externals/content/scripting/lobbythreaded/systems/?.lua"

require "sh_pickboxsystem"
require "sh_buttonhoversystem"
require "sh_buttonpressedsystem"

--if Server then
	require "serverlobbysystem"
	require "serverconnectsystem"
--end

--if Client then
	require "clientlobbysystem"
	require "clientconnectsystem"
--end

-- Templates
package.path = package.path .. ";../../../Externals/content/scripting/lobbythreaded/templates/?.lua"
require "templates"

worldCreator:AddSystemGroup()
worldCreator:AddSystemToCurrentGroup(PickBoxSystem)
worldCreator:AddSystemToCurrentGroup(ServerLobbySystem)

worldCreator:AddSystemGroup()
worldCreator:AddSystemToCurrentGroup(ButtonPressedSystem)

worldCreator:AddSystemGroup()
worldCreator:AddSystemToCurrentGroup(ButtonHoverSystem)

--if Server then
	worldCreator:AddSystemGroup()
	worldCreator:AddSystemToCurrentGroup(ServerConnectSystem)
--end

--if Client then
	worldCreator:AddSystemGroup()
	worldCreator:AddSystemToCurrentGroup(ClientLobbySystem)
	worldCreator:AddSystemGroup()
	worldCreator:AddSystemToCurrentGroup(ClientConnectSystem)
--end