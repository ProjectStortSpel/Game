-- Components
package.path = package.path .. ";components/?.lua"
require "components"
require "buttoncomponents"
require "menucomponents"
require "servercomponents"
require "ipconnectcomponents"

--	Shared Components
package.path = package.path .. ";../shared/components/?.lua"
require "InitSharedComponents" -- Add more systems into InitSharedSystems.lua instead of here!

if Client then

--	Shared Systems
package.path = package.path .. ";../shared/systems/?.lua"
require "InitSharedSystems" -- Add more systems into InitSharedSystems.lua instead of here!


-- Systems
package.path = package.path .. ";systems/?.lua"

--require "sh_pickboxsystem"
--require "sh_buttonhoversystem"
--require "sh_buttonpressedsystem"

package.path = package.path .. ";systems/interfacesystems/?.lua"
require "GameInterfaceSystem"
require "MainMenuSystem"
require "GameMenuSystem"
require "OptionMenuSystem"
require "ConnectMenuSystem"
require "HostMenuSystem"
require "IPConnectSystem"
require "HowToSystem"
require "SliderSystem"

package.path = package.path .. ";systems/Misc/?.lua"
require "PlayerNameSystem"

-- Templates
package.path = package.path .. ";templates/?.lua"
require "templates"

--	Misc
package.path = package.path .. ";misc/?.lua"
require "InitMisc"

worldCreator:AddSystemGroup()
worldCreator:AddSystemToCurrentGroup(GameInterfaceSystem)
worldCreator:AddSystemGroup()	
worldCreator:AddSystemToCurrentGroup(MainMenuSystem)
worldCreator:AddSystemGroup()
worldCreator:AddSystemToCurrentGroup(GameMenuSystem)
worldCreator:AddSystemGroup()
worldCreator:AddSystemToCurrentGroup(OptionMenuSystem)
worldCreator:AddSystemGroup()
worldCreator:AddSystemToCurrentGroup(ConnectMenuSystem)
worldCreator:AddSystemGroup()
worldCreator:AddSystemToCurrentGroup(HostMenuSystem)
worldCreator:AddSystemGroup()
worldCreator:AddSystemToCurrentGroup(SliderSystem)
worldCreator:AddSystemGroup()
worldCreator:AddSystemToCurrentGroup(IPConnectSystem)
worldCreator:AddSystemGroup()
worldCreator:AddSystemToCurrentGroup(HowToSystem)
worldCreator:AddSystemGroup()
worldCreator:AddSystemToCurrentGroup(PlayerNameSystem)

end