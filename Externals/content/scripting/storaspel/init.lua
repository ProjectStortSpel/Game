-- Components
package.path = package.path .. ";../../../Externals/content/scripting/storaspel/components/?.lua"
require "components"

-- Systems
package.path = package.path .. ";../../../Externals/content/scripting/storaspel/systems/?.lua"
require "movementsystem"

-- Templates
package.path = package.path .. ";../../../Externals/content/scripting/storaspel/templates/?.lua"
require "box"

worldCreator:AddSystemGroup()
worldCreator:AddSystemToCurrentGroup(MovementSystem)