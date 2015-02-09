-- Components
package.path = package.path .. ";../../../Externals/content/scripting/lobbythreaded/components/?.lua"
require "components"
require "buttoncomponents"
require "menucomponents"
require "servercomponents"

--	Shared Components
package.path = package.path .. ";../../../Externals/content/scripting/shared/components/?.lua"
require "InitSharedComponents" -- Add more systems into InitSharedSystems.lua instead of here!

if Client then

--	Shared Systems
package.path = package.path .. ";../../../Externals/content/scripting/shared/systems/?.lua"
require "InitSharedSystems" -- Add more systems into InitSharedSystems.lua instead of here!


-- Systems
package.path = package.path .. ";../../../Externals/content/scripting/lobbythreaded/systems/?.lua"

--require "sh_pickboxsystem"
--require "sh_buttonhoversystem"
--require "sh_buttonpressedsystem"

package.path = package.path .. ";../../../Externals/content/scripting/lobbythreaded/systems/interfacesystems/?.lua"
require "GameInterfaceSystem"
require "GameMenuSystem"
require "OptionMenuSystem"
require "ConnectMenuSystem"
require "HostMenuSystem"
require "HostMenu2System"


-- Templates
package.path = package.path .. ";../../../Externals/content/scripting/lobbythreaded/templates/?.lua"
require "templates"

--worldCreator:AddSystemGroup()
--worldCreator:AddSystemToCurrentGroup(PickBoxSystem)

--worldCreator:AddSystemGroup()
--worldCreator:AddSystemToCurrentGroup(ButtonPressedSystem)

--worldCreator:AddSystemGroup()
--worldCreator:AddSystemToCurrentGroup(ButtonHoverSystem)

worldCreator:AddSystemGroup()	
worldCreator:AddSystemToCurrentGroup(GameInterfaceSystem)
worldCreator:AddSystemGroup()
worldCreator:AddSystemToCurrentGroup(GameMenuSystem)
worldCreator:AddSystemGroup()
worldCreator:AddSystemToCurrentGroup(OptionMenuSystem)
worldCreator:AddSystemGroup()
worldCreator:AddSystemToCurrentGroup(ConnectMenuSystem)
worldCreator:AddSystemGroup()
worldCreator:AddSystemToCurrentGroup(HostMenu2System)

end