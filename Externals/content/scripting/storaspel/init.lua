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
require "cardsystem"
require "discosystem"

-- Templates
package.path = package.path .. ";../../../Externals/content/scripting/storaspel/templates/?.lua"
require "box"
require "lights"
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
worldCreator:AddSystemToCurrentGroup(RespawnSystem)
worldCreator:AddSystemToCurrentGroup(NewRoundSystem)
worldCreator:AddSystemToCurrentGroup(NewStepSystem)
worldCreator:AddSystemToCurrentGroup(WaterMovementSystem)
worldCreator:AddSystemToCurrentGroup(CardDeckSystem)
--worldCreator:AddSystemToCurrentGroup(GetCardsSystem)

worldCreator:AddSystemToCurrentGroup(DiscoSystem)