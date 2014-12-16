-- Turn Left Component
local TurnLeftComponent = ComponentType()
TurnLeftComponent.Name = "TurnLeft"
TurnLeftComponent.TableType = TableType.Array
worldCreator:AddComponentType(TurnLeftComponent)

-- Turn Right Component
local TurnRightComponent = ComponentType()
TurnRightComponent.Name = "TurnRight"
TurnRightComponent.TableType = TableType.Array
worldCreator:AddComponentType(TurnRightComponent)

-- Turn Around Component
local TurnAroundComponent = ComponentType()
TurnAroundComponent.Name = "TurnAround"
TurnAroundComponent.TableType = TableType.Array
worldCreator:AddComponentType(TurnAroundComponent)

-- Forward Component
local ForwardComponent = ComponentType()
ForwardComponent.Name = "Forward"
ForwardComponent.TableType = TableType.Array
worldCreator:AddComponentType(ForwardComponent)

-- Backward Component
local BackwardComponent = ComponentType()
BackwardComponent.Name = "Backward"
BackwardComponent.TableType = TableType.Array
worldCreator:AddComponentType(BackwardComponent)