-- Components
package.path = package.path .. ";../../../Externals/content/scripting/lobbythreaded/components/?.lua"
require "components"
require "buttoncomponents"
require "menucomponents"
require "servercomponents"

package.path = package.path .. ";../../../Externals/content/scripting/shared/components/?.lua"
require "interfacecomponents"
require "textrendercomponents"
require "lightcomponents"
require "cameracomponents"
require "components"

-- Systems
package.path = package.path .. ";../../../Externals/content/scripting/lobbythreaded/systems/?.lua"

--require "sh_pickboxsystem"
--require "sh_buttonhoversystem"
--require "sh_buttonpressedsystem"

--if Server then
	require "serverlobbysystem"
	require "serverconnectsystem"
--end

--if Client then
	require "clientlobbysystem"
	require "clientconnectsystem"
--end

package.path = package.path .. ";../../../Externals/content/scripting/lobbythreaded/systems/interfacesystems/?.lua"
require "gameinterfacesystem"
require "gamemenusystem"
require "optionmenusystem"
require "connectmenusystem"
require "hostmenusystem"

package.path = package.path .. ";../../../Externals/content/scripting/shared/systems/?.lua"
require "pickboxsystem"
require "hoversizesystem"
require "addtexttotexturesystem"
require "parentsystem"

-- Templates
package.path = package.path .. ";../../../Externals/content/scripting/lobbythreaded/templates/?.lua"
require "templates"

--worldCreator:AddSystemGroup()
--worldCreator:AddSystemToCurrentGroup(PickBoxSystem)

worldCreator:AddSystemGroup()
worldCreator:AddSystemToCurrentGroup(ParentSystem)
worldCreator:AddSystemGroup()
worldCreator:AddSystemToCurrentGroup(PickBoxSystem)
worldCreator:AddSystemGroup()
worldCreator:AddSystemToCurrentGroup(HoverSizeSystem)
worldCreator:AddSystemGroup()
worldCreator:AddSystemToCurrentGroup(AddTextToTextureSystem)

--worldCreator:AddSystemGroup()
--worldCreator:AddSystemToCurrentGroup(ButtonPressedSystem)

--worldCreator:AddSystemGroup()
--worldCreator:AddSystemToCurrentGroup(ButtonHoverSystem)


--if Server then
	worldCreator:AddSystemGroup()
	--worldCreator:AddSystemToCurrentGroup(ButtonHoverSystem)
	worldCreator:AddSystemToCurrentGroup(ServerLobbySystem)
	worldCreator:AddSystemGroup()
	worldCreator:AddSystemToCurrentGroup(ServerConnectSystem)
--end

--if Client then
	worldCreator:AddSystemGroup()
	worldCreator:AddSystemToCurrentGroup(ClientLobbySystem)
	worldCreator:AddSystemGroup()
	worldCreator:AddSystemToCurrentGroup(ClientConnectSystem)
--end

worldCreator:AddSystemGroup()	
worldCreator:AddSystemToCurrentGroup(GameInterfaceSystem)
worldCreator:AddSystemGroup()
worldCreator:AddSystemToCurrentGroup(GameMenuSystem)
worldCreator:AddSystemGroup()
worldCreator:AddSystemToCurrentGroup(OptionMenuSystem)
worldCreator:AddSystemGroup()
worldCreator:AddSystemToCurrentGroup(ConnectMenuSystem)
worldCreator:AddSystemGroup()
worldCreator:AddSystemToCurrentGroup(HostMenuSystem)