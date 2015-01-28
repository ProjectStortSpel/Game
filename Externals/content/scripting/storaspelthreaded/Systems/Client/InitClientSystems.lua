--	Misc Systems
package.path = package.path .. ";../../../Externals/content/scripting/storaspelthreaded/Systems/Client/MiscSystems/?.lua"
require "cl_PlayerIndicatorSystem"

worldCreator:AddSystemGroup()
worldCreator:AddSystemToCurrentGroup(PlayerIndicatorSystem)

--	Interface Systems
package.path = package.path .. ";../../../Externals/content/scripting/storaspelthreaded/Systems/Client/InterfaceSystems/?.lua"
require "cl_RconMenuSystem"
require "cl_OptionsMenuSystem"
--require "cl_HoverSizeSystem"
require "cl_GameMenuSystem"
require "cl_GameInterfaceSystem"

worldCreator:AddSystemGroup()
worldCreator:AddSystemToCurrentGroup(RconMenuSystem)
worldCreator:AddSystemGroup()
worldCreator:AddSystemToCurrentGroup(OptionMenuSystem)
worldCreator:AddSystemGroup()
worldCreator:AddSystemToCurrentGroup(GameMenuSystem)
worldCreator:AddSystemGroup()
worldCreator:AddSystemToCurrentGroup(GameInterfaceSystem)
--worldCreator:AddSystemGroup()
--worldCreator:AddSystemToCurrentGroup(HoverSizeSystem)

--	Card Systems
package.path = package.path .. ";../../../Externals/content/scripting/storaspelthreaded/Systems/Client/CardSystems/?.lua"
require "cl_CardAddModelSystem"
require "cl_GiveCardIndexSystem"
require "cl_CardHoverSystem"
require "cl_CardPositionSystem"
require "cl_CardPickedPositionSystem"
require "cl_CardSelectSystem"
require "cl_SortSelectedCardsSystem"
require "cl_SortCardIndexSystem"
require "cl_SendSelectedCardsSystem"


worldCreator:AddSystemGroup()
worldCreator:AddSystemToCurrentGroup(GiveCardIndexSystem)
worldCreator:AddSystemGroup()
worldCreator:AddSystemToCurrentGroup(SortCardIndexSystem)
worldCreator:AddSystemGroup()
worldCreator:AddSystemToCurrentGroup(SortSelectedCardsSystem)
worldCreator:AddSystemGroup()
worldCreator:AddSystemToCurrentGroup(SendSelectedCardsSystem)
worldCreator:AddSystemGroup()
worldCreator:AddSystemToCurrentGroup(CardPositionSystem)
worldCreator:AddSystemGroup()
worldCreator:AddSystemToCurrentGroup(CardPickedPositionSystem)
worldCreator:AddSystemGroup()
worldCreator:AddSystemToCurrentGroup(CardAddModelSystem)
worldCreator:AddSystemGroup()
worldCreator:AddSystemToCurrentGroup(CardHoverSystem)
worldCreator:AddSystemGroup()
worldCreator:AddSystemToCurrentGroup(CardSelectSystem)

--	Camera Systems
package.path = package.path .. ";../../../Externals/content/scripting/storaspelthreaded/Systems/Client/CameraSystems/?.lua"
require "cl_NewCameraSystem"

worldCreator:AddSystemGroup()
worldCreator:AddSystemToCurrentGroup(NewCameraSystem)
