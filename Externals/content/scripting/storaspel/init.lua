-- Components
package.path = package.path .. ";../../../Externals/content/scripting/storaspel/components/?.lua"
require "components"

-- Systems
package.path = package.path .. ";../../../Externals/content/scripting/storaspel/systems/?.lua"
require "movementsystem"

--worldCreator:AddSystemGroup()
--worldCreator:AddSystemToCurrentGroup(MovementSystem)