--	This file will contain
--	components that are needed
--	for player components

--	Player Component
local PlayerComponent = ComponentType()
PlayerComponent.Name = "Player"
PlayerComponent.TableType = TableType.Map
worldCreator:AddComponentType(PlayerComponent)

--	PlayerNumber Component
local PlayerNumberComponent = ComponentType()
PlayerNumberComponent.Name = "PlayerNumber"
PlayerNumberComponent.TableType = TableType.Map
PlayerNumberComponent:AddVariable("Number", ByteSize.Int)
worldCreator:AddComponentType(PlayerNumberComponent)

--	PlayerName Component
local PlayerNameComponent = ComponentType()
PlayerNameComponent.Name = "PlayerName"
PlayerNameComponent.TableType = TableType.Map
PlayerNameComponent:AddVariable("Name", ByteSize.Text)
worldCreator:AddComponentType(PlayerNameComponent)

-- Inactive Player Component
local InactivePlayerComponent = ComponentType()
InactivePlayerComponent.Name = "InactivePlayer"
InactivePlayerComponent.TableType = TableType.Map
worldCreator:AddComponentType(InactivePlayerComponent)

--	UnitEntityId Component
local UnitEntityIdComponent = ComponentType()
UnitEntityIdComponent.Name = "UnitEntityId"
UnitEntityIdComponent.TableType = TableType.Map
UnitEntityIdComponent:AddVariable("Id", ByteSize.Reference)
worldCreator:AddComponentType(UnitEntityIdComponent)

-- IsSpectator Component
local IsSpectator = ComponentType()
IsSpectator.Name = "IsSpectator"
IsSpectator.TableType = TableType.Map
worldCreator:AddComponentType(IsSpectator)

-- PlayerIndicator Component
local PlayerIndicator = ComponentType()
PlayerIndicator.Name = "PlayerIndicator"
PlayerIndicator.TableType = TableType.Map
worldCreator:AddComponentType(PlayerIndicator)

--	PickingDone Component
local PickingDone = ComponentType()
PickingDone.Name = "PickingDone"
PickingDone.TableType = TableType.Map
PickingDone:AddVariable("Done", ByteSize.Int)
worldCreator:AddComponentType(PickingDone)

-- HasSelectedCards Component
local HasSelectedCardsComponent = ComponentType()
HasSelectedCardsComponent.Name = "HasSelectedCards"
HasSelectedCardsComponent.TableType = TableType.Map
worldCreator:AddComponentType(HasSelectedCardsComponent)

-- PlayerCounter Component
local PlayerCounterComponent = ComponentType()
PlayerCounterComponent.Name = "PlayerCounter"
PlayerCounterComponent.TableType = TableType.Map
PlayerCounterComponent:AddVariable("MaxPlayers", ByteSize.Int)
PlayerCounterComponent:AddVariable("Players", ByteSize.Int)
PlayerCounterComponent:AddVariable("Spectators", ByteSize.Int)
worldCreator:AddComponentType(PlayerCounterComponent)

-- NoSubSteps Component
local NoSubStepsComponent = ComponentType()
NoSubStepsComponent.Name = "NoSubSteps"
NoSubStepsComponent.TableType = TableType.Map
NoSubStepsComponent:AddVariable("Counter", ByteSize.Int)
worldCreator:AddComponentType(NoSubStepsComponent)