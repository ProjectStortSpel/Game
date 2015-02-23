--	Misc Systems
package.path = package.path .. ";Systems/Client/MiscSystems/?.lua"
require "cl_PlayerIndicatorSystem"
require "cl_ClientNetworkMessageSystem"
require "cl_FlowingWaterSystem"
require "cl_ChatSystem"
require "cl_ShowNextCheckpointSystem"

--	Interface Systems
package.path = package.path .. ";Systems/Client/InterfaceSystems/?.lua"
require "cl_RconMenuSystem"
require "cl_OptionsMenuSystem"
require "cl_GameMenuSystem"
require "cl_GameInterfaceSystem"
require "cl_PickingTimerSystem"

--	Card Systems
package.path = package.path .. ";Systems/Client/CardSystems/?.lua"
require "cl_CardAddModelSystem"
require "cl_GiveCardIndexSystem"
require "cl_CardHoverSystem"
require "cl_CardPositionSystem"
require "cl_CardPickedPositionSystem"
require "cl_CardSelectSystem"
require "cl_SortSelectedCardsSystem"
require "cl_SortCardIndexSystem"
require "cl_SendSelectedCardsSystem"

--	Camera Systems
package.path = package.path .. ";Systems/Client/CameraSystems/?.lua"
require "cl_NewCameraSystem"

worldCreator:AddSystemGroup()
worldCreator:AddSystemToCurrentGroup(ClientNetworkMessageSystem)


worldCreator:AddSystemGroup()
worldCreator:AddSystemToCurrentGroup(SendSelectedCardsSystem)
worldCreator:AddSystemToCurrentGroup(OptionMenuSystem)
worldCreator:AddSystemToCurrentGroup(CardPositionSystem)
worldCreator:AddSystemToCurrentGroup(GiveCardIndexSystem)
worldCreator:AddSystemToCurrentGroup(ChatSystem)
worldCreator:AddSystemToCurrentGroup(SortCardIndexSystem)
worldCreator:AddSystemToCurrentGroup(PickingTimerSystem)
worldCreator:AddSystemToCurrentGroup(GameInterfaceSystem)
worldCreator:AddSystemToCurrentGroup(PlayerIndicatorSystem)
worldCreator:AddSystemToCurrentGroup(CardSelectSystem)
worldCreator:AddSystemToCurrentGroup(NewCameraSystem)
worldCreator:AddSystemToCurrentGroup(CardPickedPositionSystem)
worldCreator:AddSystemToCurrentGroup(RconMenuSystem)
worldCreator:AddSystemToCurrentGroup(FlowingWaterSystem)
worldCreator:AddSystemToCurrentGroup(SortSelectedCardsSystem)
worldCreator:AddSystemToCurrentGroup(GameMenuSystem)
worldCreator:AddSystemToCurrentGroup(CardHoverSystem)
worldCreator:AddSystemToCurrentGroup(CardAddModelSystem)



















--[[
worldCreator:AddSystemGroup()
worldCreator:AddSystemToCurrentGroup(PlayerIndicatorSystem)

worldCreator:AddSystemToCurrentGroup(FlowingWaterSystem)
worldCreator:AddSystemToCurrentGroup(ChatSystem)


worldCreator:AddSystemToCurrentGroup(ShowNextCheckpointSystem)

worldCreator:AddSystemToCurrentGroup(RconMenuSystem)
worldCreator:AddSystemToCurrentGroup(OptionMenuSystem)
worldCreator:AddSystemToCurrentGroup(GameMenuSystem)
worldCreator:AddSystemToCurrentGroup(GameInterfaceSystem)
worldCreator:AddSystemToCurrentGroup(PickingTimerSystem)




worldCreator:AddSystemToCurrentGroup(GiveCardIndexSystem)
worldCreator:AddSystemToCurrentGroup(SortCardIndexSystem)
worldCreator:AddSystemToCurrentGroup(SortSelectedCardsSystem)
worldCreator:AddSystemToCurrentGroup(SendSelectedCardsSystem)
worldCreator:AddSystemToCurrentGroup(CardPositionSystem)
worldCreator:AddSystemToCurrentGroup(CardPickedPositionSystem)
worldCreator:AddSystemToCurrentGroup(CardAddModelSystem)
worldCreator:AddSystemToCurrentGroup(CardHoverSystem)
worldCreator:AddSystemToCurrentGroup(CardSelectSystem)



worldCreator:AddSystemToCurrentGroup(NewCameraSystem)
--]]
