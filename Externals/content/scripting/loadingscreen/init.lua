-- Components
package.path = package.path .. ";components/?.lua"
require "components"

--	Shared Components
package.path = package.path .. ";../shared/components/?.lua"
require "InitSharedComponents" -- Add more systems into InitSharedSystems.lua instead of here!

if Client then

--	Shared Systems
package.path = package.path .. ";../shared/systems/?.lua"
require "InitSharedSystems" -- Add more systems into InitSharedSystems.lua instead of here!

-- Systems

package.path = package.path .. ";systems/interfacesystems/?.lua"
require "LoadingbarSystem"

worldCreator:AddSystemGroup()
worldCreator:AddSystemToCurrentGroup(LoadingbarSystem)

end