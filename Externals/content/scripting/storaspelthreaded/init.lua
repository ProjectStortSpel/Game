--	MaxNumberOfEntities
worldCreator:SetMaxNumberOfEntities(10000)

--	Game Components
package.path = package.path .. ";Components/?.lua"
require "InitComponents" -- Add more components into InitComponents.lua instead of here!

--	Shared Components
package.path = package.path .. ";../shared/components/?.lua"
require "InitSharedComponents" -- Add more systems into InitSharedSystems.lua instead of here!

--	Shared Systems
package.path = package.path .. ";../shared/systems/?.lua"
require "InitSharedSystems" -- Add more systems into InitSharedSystems.lua instead of here!

--	Shared Misc
package.path = package.path .. ";../shared/misc/?.lua"
require "InitMisc"

--	Templates
package.path = package.path .. ";Templates/?.lua"
require "InitTemplates" -- Add more templates into InitTemplates.lua instead of here!

--	Client Systems
if Client then
	package.path = package.path .. ";Systems/Client/?.lua"
	require "InitClientSystems" -- Add more client systems into InitClientSystems.lua instead of here!
end

--	Server Systems
if Server then
	package.path = package.path .. ";Systems/Server/?.lua"
	require "InitServerSystems" -- Add more client systems into InitServerSystems.lua instead of here!
end
--	Shared Game Systems
package.path = package.path .. ";Systems/Shared/?.lua"
require "InitSharedSystems" -- Add more client systems into InitSharedSystems.lua instead of here!

--	Misc
package.path = package.path .. ";Misc/?.lua"
require "InitMisc"

-- LoadingScreen
if Server then
	LoadingScreen.SetAccessLevel(1)
elseif Client then
	LoadingScreen.SetAccessLevel(2)	
	
	Net.Receive("RemoveLoadingScreen", function(id, ip, port)
		local id = world:CreateNewEntity()
		world:CreateComponentAndAddTo("LoadingScreenDelay", id)
		world:GetComponent(id, "LoadingScreenDelay", "Delay"):SetInt(5)
		world:GetComponent(id, "LoadingScreenDelay", "AccessLevel"):SetInt(2)
	end)
	
end