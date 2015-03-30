--	Misc Systems
package.path = package.path .. ";Systems/Client/MiscSystems/?.lua"
require "cl_PlayerIndicatorSystem"
require "cl_ClientNetworkMessageSystem"
require "cl_FlowingWaterSystem"
require "cl_ChatSystem"
require "cl_GlowNextCheckpointSystem"
require "cl_ShowNextCheckpointSystem"
require "cl_LobbySystem"
require "cl_VisualizeSelectedCards"
require "cl_FadeTreeSystem"
require "cl_AbilityStoneSystem"
require "cl_AbilitySlingshotSystem"
require "cl_WinScreenSystem"

--	Interface Systems
package.path = package.path .. ";Systems/Client/InterfaceSystems/?.lua"
require "cl_RconMenuSystem"
require "cl_OptionsMenuSystem"
require "cl_GameMenuSystem"
require "cl_GameInterfaceSystem"
require "cl_PickingTimerSystem"
require "cl_NotificationSystem"
require "cl_ScoreboardSystem"
require "cl_SliderSystem"

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

--	Audio System
package.path = package.path .. ";Systems/Client/AudioSystems/?.lua"
require "cl_AudioRiverSystem"

--	Audio System
package.path = package.path .. ";Systems/Client/WeatherSystems/?.lua"
require "cl_WeatherRainSystem"

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
worldCreator:AddSystemToCurrentGroup(NotificationSystem)
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
worldCreator:AddSystemToCurrentGroup(ScoreboardSystem)
worldCreator:AddSystemToCurrentGroup(SliderSystem)
worldCreator:AddSystemToCurrentGroup(ClientAbilityStoneSystem)
worldCreator:AddSystemToCurrentGroup(ClientAbilitySlingshotSystem)

worldCreator:AddSystemToCurrentGroup(LobbySystem)
worldCreator:AddSystemToCurrentGroup(CardAddModelSystem)
worldCreator:AddSystemToCurrentGroup(ShowNextCheckpointSystem)
worldCreator:AddSystemToCurrentGroup(GlowNextCheckpointSystem)

worldCreator:AddSystemToCurrentGroup(AudioRiverSystem)

worldCreator:AddSystemGroup()
worldCreator:AddSystemToCurrentGroup(VisualizeSelectedCards)
worldCreator:AddSystemToCurrentGroup(FadeTreeSystem)

worldCreator:AddSystemToCurrentGroup(WinScreenSystem)


worldCreator:AddSystemToCurrentGroup(WeatherRainSystem)











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
