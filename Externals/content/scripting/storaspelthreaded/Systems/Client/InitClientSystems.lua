--	Misc Systems
package.path = package.path .. ";../../../Externals/content/scripting/storaspelthreaded/Systems/Client/MiscSystems/?.lua"
require "cl_PlayerIndicatorSystem"

worldCreator:AddSystemGroup()
worldCreator:AddSystemToCurrentGroup(PlayerIndicatorSystem)

--	Interface Systems
package.path = package.path .. ";../../../Externals/content/scripting/storaspelthreaded/Systems/Client/InterfaceSystems/?.lua"
require "cl_RconMenuSystem"
require "cl_OptionsMenuSystem"
require "cl_HoverSizeSystem"
require "cl_GameMenuSystem"
require "cl_GameInterfaceSystem"

worldCreator:AddSystemGroup()
worldCreator:AddSystemToCurrentGroup(PlayerIndicatorSystem)
worldCreator:AddSystemGroup()
worldCreator:AddSystemToCurrentGroup(OptionMenuSystem)
worldCreator:AddSystemGroup()
worldCreator:AddSystemToCurrentGroup(HoverSizeSystem)
worldCreator:AddSystemGroup()
worldCreator:AddSystemToCurrentGroup(GameMenuSystem)
worldCreator:AddSystemGroup()
worldCreator:AddSystemToCurrentGroup(GameInterfaceSystem)

--	Card Systems
package.path = package.path .. ";../../../Externals/content/scripting/storaspelthreaded/Systems/Client/InterfaceSystems/?.lua"
require "cl_RconMenuSystem"
require "cl_OptionsMenuSystem"
require "cl_HoverSizeSystem"
require "cl_GameMenuSystem"
require "cl_GameInterfaceSystem"

worldCreator:AddSystemGroup()
worldCreator:AddSystemToCurrentGroup(PlayerIndicatorSystem)
worldCreator:AddSystemGroup()
worldCreator:AddSystemToCurrentGroup(OptionMenuSystem)
worldCreator:AddSystemGroup()
worldCreator:AddSystemToCurrentGroup(HoverSizeSystem)
worldCreator:AddSystemGroup()
worldCreator:AddSystemToCurrentGroup(GameMenuSystem)
worldCreator:AddSystemGroup()
worldCreator:AddSystemToCurrentGroup(GameInterfaceSystem)