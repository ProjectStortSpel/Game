-- Components
package.path = package.path .. ";../../../Externals/content/scripting/storaspel/components/?.lua"
require "components"
require "lightcomponents"
require "maplogiccomponents"
require "playerlogiccomponents"


-- Systems
package.path = package.path .. ";../../../Externals/content/scripting/storaspel/systems/?.lua"
require "movementsystem"
require "networkmessagessystem"
require "mapcreationsystem"
require "moveplayersystem"

-- Templates
package.path = package.path .. ";../../../Externals/content/scripting/storaspel/templates/?.lua"
require "box"
require "map"

worldCreator:AddSystemGroup()
worldCreator:AddSystemToCurrentGroup(MovementSystem)
worldCreator:AddSystemToCurrentGroup(networkMessagesSystem)
worldCreator:AddSystemToCurrentGroup(MapCreationSystem)
worldCreator:AddSystemToCurrentGroup(ForwardSystem)
worldCreator:AddSystemToCurrentGroup(BackwardSystem)
worldCreator:AddSystemToCurrentGroup(RightTurnSystem)
worldCreator:AddSystemToCurrentGroup(LeftTurnSystem)
worldCreator:AddSystemToCurrentGroup(TurnAroundSystem)
worldCreator:AddSystemToCurrentGroup(TestMovementSystem)