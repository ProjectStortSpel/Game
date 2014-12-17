-- Components
package.path = package.path .. ";../../../Externals/content/scripting/lobby/components/?.lua"
require "components"

-- Systems
package.path = package.path .. ";../../../Externals/content/scripting/lobby/systems/?.lua"
require "systems"

--if Server then
	--require "serverlobbysystem"
	require "serverconnectsystem"
--end


-- Templates
package.path = package.path .. ";../../../Externals/content/scripting/lobby/templates/?.lua"
require "templates"

worldCreator:AddSystemGroup()
worldCreator:AddSystemToCurrentGroup(StartUpSystem)
--worldCreator:AddSystemToCurrentGroup(ServerLobbySystem)
worldCreator:AddSystemToCurrentGroup(ServerConnectSystem)