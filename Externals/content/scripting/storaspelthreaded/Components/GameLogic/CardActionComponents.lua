--	This file will contain
--	components that are needed
--	for the card action

-- Turn Left Component
local UnitTurnLeftComponent = ComponentType()
UnitTurnLeftComponent.Name = "UnitTurnLeft"
UnitTurnLeftComponent.TableType = TableType.Map
worldCreator:AddComponentType(UnitTurnLeftComponent)

-- Turn Right Component
local UnitTurnRightComponent = ComponentType()
UnitTurnRightComponent.Name = "UnitTurnRight"
UnitTurnRightComponent.TableType = TableType.Map
worldCreator:AddComponentType(UnitTurnRightComponent)

-- Turn Around Component
local UnitTurnAroundComponent = ComponentType()
UnitTurnAroundComponent.Name = "UnitTurnAround"
UnitTurnAroundComponent.TableType = TableType.Map
worldCreator:AddComponentType(UnitTurnAroundComponent)

-- Forward Component
local UnitForwardComponent = ComponentType()
UnitForwardComponent.Name = "UnitForward"
UnitForwardComponent.TableType = TableType.Map
worldCreator:AddComponentType(UnitForwardComponent)

-- Backward Component
local UnitBackwardComponent = ComponentType()
UnitBackwardComponent.Name = "UnitBackward"
UnitBackwardComponent.TableType = TableType.Map
worldCreator:AddComponentType(UnitBackwardComponent)

-- UnitGuard Component
local UnitGuardComponent = ComponentType()
UnitGuardComponent.Name = "UnitGuard"
UnitGuardComponent.TableType = TableType.Map
worldCreator:AddComponentType(UnitGuardComponent)

-- UnitSlingShot Component
local UnitSlingShotComponent = ComponentType()
UnitSlingShotComponent.Name = "UnitSlingShot"
UnitSlingShotComponent.TableType = TableType.Map
worldCreator:AddComponentType(UnitSlingShotComponent)

-- Sprint Component
local UnitSprintComponent = ComponentType()
UnitSprintComponent.Name = "UnitSprint"
UnitSprintComponent.TableType = TableType.Map
worldCreator:AddComponentType(UnitSprintComponent)

-- IWin Component
local UnitIWin = ComponentType()
UnitIWin.Name = "UnitIWin"
UnitIWin.TableType = TableType.Map
worldCreator:AddComponentType(UnitIWin)