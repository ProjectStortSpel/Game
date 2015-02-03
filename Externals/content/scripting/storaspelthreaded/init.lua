--	Game Components
package.path = package.path .. ";../../../Externals/content/scripting/storaspelthreaded/Components/?.lua"
require "InitComponents" -- Add more components into InitComponents.lua instead of here!
--	Shared Components
package.path = package.path .. ";../../../Externals/content/scripting/shared/components/?.lua"
require "servercomponents" -- Add more components into InitComponents.lua instead of here!
require "interfacecomponents" -- Add more components into InitComponents.lua instead of here!


--	Templates
package.path = package.path .. ";../../../Externals/content/scripting/storaspelthreaded/Templates/?.lua"
require "InitTemplates" -- Add more templates into InitTemplates.lua instead of here!

--	Client Systems
if Client then
	package.path = package.path .. ";../../../Externals/content/scripting/storaspelthreaded/Systems/Client/?.lua"
	require "InitClientSystems" -- Add more client systems into InitClientSystems.lua instead of here!
end

--	Server Systems
if Server then
	package.path = package.path .. ";../../../Externals/content/scripting/storaspelthreaded/Systems/Server/?.lua"
	require "InitServerSystems" -- Add more client systems into InitServerSystems.lua instead of here!
end
--	Shared Game Systems
package.path = package.path .. ";../../../Externals/content/scripting/storaspelthreaded/Systems/Shared/?.lua"
require "InitSharedSystems" -- Add more client systems into InitSharedSystems.lua instead of here!

--	Shared Systems
package.path = package.path .. ";../../../Externals/content/scripting/shared/systems/?.lua"
require "sh_HoverSizeSystem" -- Add more components into InitComponents.lua instead of here!
require "sh_PickboxSystem" -- Add more components into InitComponents.lua instead of here!

worldCreator:AddSystemGroup()
worldCreator:AddSystemToCurrentGroup(HoverSizeSystem)

worldCreator:AddSystemGroup()
worldCreator:AddSystemToCurrentGroup(PickBoxSystem)