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

if Server then
require "mapcreationsystem"
end
require "maplogicsystem"

require "moveplayersystem"
require "networkconnectsystem"
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
if Server then
worldCreator:AddSystemToCurrentGroup(MapCreationSystem)
end
worldCreator:AddSystemToCurrentGroup(ForwardSystem)
worldCreator:AddSystemToCurrentGroup(BackwardSystem)
worldCreator:AddSystemToCurrentGroup(RightTurnSystem)
worldCreator:AddSystemToCurrentGroup(LeftTurnSystem)
worldCreator:AddSystemToCurrentGroup(TurnAroundSystem)
worldCreator:AddSystemToCurrentGroup(PlayerMovementSystem)
worldCreator:AddSystemToCurrentGroup(RespawnSystem)
worldCreator:AddSystemToCurrentGroup(NewRoundSystem)
worldCreator:AddSystemToCurrentGroup(NetworkConnectSystem)
worldCreator:AddSystemToCurrentGroup(NewStepSystem)
worldCreator:AddSystemToCurrentGroup(WaterMovementSystem)
worldCreator:AddSystemToCurrentGroup(CardDeckSystem)
--worldCreator:AddSystemToCurrentGroup(GetCardsSystem)

worldCreator:AddSystemToCurrentGroup(DiscoSystem)