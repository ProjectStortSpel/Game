-- Components
package.path = package.path .. ";../../../Externals/content/scripting/storaspel/components/?.lua"
require "components"
require "lightcomponents"
require "mapcomponents"
require "playercomponents"
require "cardcomponents"


-- Systems
package.path = package.path .. ";../../../Externals/content/scripting/storaspel/systems/?.lua"
require "movementsystem"
require "networkmessagessystem"
require "mapcreationsystem"
require "maplogicsystem"
require "moveplayersystem"
require "cardlogicsystem"
require "discosystem"

-- Templates
package.path = package.path .. ";../../../Externals/content/scripting/storaspel/templates/?.lua"
require "box"
require "lights"
require "map"

worldCreator:AddSystemGroup()
worldCreator:AddSystemToCurrentGroup(MovementSystem)
worldCreator:AddSystemToCurrentGroup(networkMessagesSystem)
worldCreator:AddSystemToCurrentGroup(PlayerMovementSystem)
worldCreator:AddSystemToCurrentGroup(MapCreationSystem)

-- Map logic
worldCreator:AddSystemToCurrentGroup(NewRoundSystem)
worldCreator:AddSystemToCurrentGroup(NewStepSystem)
worldCreator:AddSystemToCurrentGroup(RespawnSystem)
worldCreator:AddSystemToCurrentGroup(WaterMovementSystem)

-- Card logic
worldCreator:AddSystemToCurrentGroup(ForwardSystem)
worldCreator:AddSystemToCurrentGroup(BackwardSystem)
worldCreator:AddSystemToCurrentGroup(RightTurnSystem)
worldCreator:AddSystemToCurrentGroup(LeftTurnSystem)
worldCreator:AddSystemToCurrentGroup(TurnAroundSystem)

worldCreator:AddSystemToCurrentGroup(CardDeckSystem)
worldCreator:AddSystemToCurrentGroup(GetCardsSystem)

worldCreator:AddSystemToCurrentGroup(DiscoSystem)