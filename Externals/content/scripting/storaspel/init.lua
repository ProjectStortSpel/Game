-- Components
package.path = package.path .. ";../../../Externals/content/scripting/storaspel/components/?.lua"
require "components"
require "tilecomponents"

-- Systems
package.path = package.path .. ";../../../Externals/content/scripting/storaspel/systems/?.lua"
require "movementsystem"
require "mapcreationsystem"
require "TestSystem"

worldCreator:AddSystemGroup()
worldCreator:AddSystemToCurrentGroup(MovementSystem)
worldCreator:AddSystemToCurrentGroup(MapCreationSystem)

worldCreator:AddSystemToCurrentGroup(ForwardSystem)
worldCreator:AddSystemToCurrentGroup(BackwardSystem)
worldCreator:AddSystemToCurrentGroup(RightTurnSystem)
worldCreator:AddSystemToCurrentGroup(LeftTurnSystem)