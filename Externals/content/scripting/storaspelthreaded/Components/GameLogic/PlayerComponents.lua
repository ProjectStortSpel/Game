--	This file will contain
--	components that are needed
--	for player components

--	Player Component
local NewComponent = ComponentType()
NewComponent.Name = "Player"
NewComponent.TableType = TableType.Map
worldCreator:AddComponentType(NewComponent)

--	PlayerNumber Component
NewComponent = ComponentType()
NewComponent.Name = "PlayerNumber"
NewComponent.TableType = TableType.Map
NewComponent:AddVariable("Number", ByteSize.Int)
worldCreator:AddComponentType(NewComponent)

--	PlayerName Component
NewComponent = ComponentType()
NewComponent.Name = "PlayerName"
NewComponent.TableType = TableType.Map
NewComponent:AddVariable("Name", ByteSize.Text)
worldCreator:AddComponentType(NewComponent)

--	PlayerNameChanged Component
NewComponent = ComponentType()
NewComponent.Name = "PlayerNameChanged"
NewComponent.TableType = TableType.Map
NewComponent:AddVariable("Name", ByteSize.String)
NewComponent:AddVariable("IpAddress", ByteSize.Text)
NewComponent:AddVariable("Port", ByteSize.Int)
worldCreator:AddComponentType(NewComponent)

--	PlayerNameRemoved Component
NewComponent = ComponentType()
NewComponent.Name = "PlayerNameRemoved"
NewComponent.TableType = TableType.Map
NewComponent:AddVariable("IpAddress", ByteSize.Text)
NewComponent:AddVariable("Port", ByteSize.Int)
worldCreator:AddComponentType(NewComponent)

-- Inactive Player Component
NewComponent = ComponentType()
NewComponent.Name = "InactivePlayer"
NewComponent.TableType = TableType.Map
worldCreator:AddComponentType(NewComponent)

--	UnitEntityId Component
NewComponent = ComponentType()
NewComponent.Name = "UnitEntityId"
NewComponent.TableType = TableType.Map
NewComponent:AddVariable("Id", ByteSize.Reference)
worldCreator:AddComponentType(NewComponent)

-- IsSpectator Component
NewComponent = ComponentType()
NewComponent.Name = "IsSpectator"
NewComponent.TableType = TableType.Map
worldCreator:AddComponentType(NewComponent)

-- PlayerIndicator Component
NewComponent = ComponentType()
NewComponent.Name = "PlayerIndicator"
NewComponent.TableType = TableType.Map
worldCreator:AddComponentType(NewComponent)

--	PickingDone Component
NewComponent = ComponentType()
NewComponent.Name = "PickingDone"
NewComponent.TableType = TableType.Map
NewComponent:AddVariable("Done", ByteSize.Int)
worldCreator:AddComponentType(NewComponent)

-- HasSelectedCards Component
NewComponent = ComponentType()
NewComponent.Name = "HasSelectedCards"
NewComponent.TableType = TableType.Map
worldCreator:AddComponentType(NewComponent)

-- PlayerCounter Component
NewComponent = ComponentType()
NewComponent.Name = "PlayerCounter"
NewComponent.TableType = TableType.Map
--PlayerCounterComponent:AddVariable("MaxPlayers", ByteSize.Int)
NewComponent:AddVariable("AIs", ByteSize.Int)
NewComponent:AddVariable("Players", ByteSize.Int)
NewComponent:AddVariable("Spectators", ByteSize.Int)
worldCreator:AddComponentType(NewComponent)

-- NoSubSteps Component
NewComponent = ComponentType()
NewComponent.Name = "NoSubSteps"
NewComponent.TableType = TableType.Map
NewComponent:AddVariable("Counter", ByteSize.Int)
worldCreator:AddComponentType(NewComponent)

-- PlayerReachedFinish Component
NewComponent = ComponentType()
NewComponent.Name = "PlayerReachedFinish"
NewComponent.TableType = TableType.Map
worldCreator:AddComponentType(NewComponent)