-- Card Component
local CardComponent = ComponentType()
CardComponent.SyncNetwork = true
CardComponent.Name = "Card"
CardComponent.TableType = TableType.Map
worldCreator:AddComponentType(CardComponent)

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

-- Card Prio Component
local CardPrioComponent = ComponentType()
CardPrioComponent.Name = "CardPrio"
CardPrioComponent.SyncNetwork = true
CardPrioComponent.TableType = TableType.Map
CardPrioComponent:AddVariable("Prio", ByteSize.Int)
worldCreator:AddComponentType(CardPrioComponent)

-- Card Action Component
local CardActionComponent = ComponentType()
CardActionComponent.Name = "CardAction"
CardActionComponent.SyncNetwork = true
CardActionComponent.TableType = TableType.Map
CardActionComponent:AddVariable("Action", ByteSize.Text)
worldCreator:AddComponentType(CardActionComponent)

-- Used Card Component
local UsedCardComponent = ComponentType()
UsedCardComponent.Name = "UsedCard"
UsedCardComponent.TableType = TableType.Map
worldCreator:AddComponentType(UsedCardComponent)

-- Dealt Card Component
local DealtCardComponent = ComponentType()
DealtCardComponent.Name = "DealtCard"
DealtCardComponent:AddVariable("PlayerEntityId", ByteSize.Reference)
DealtCardComponent.TableType = TableType.Map
worldCreator:AddComponentType(DealtCardComponent)

-- Card Step Component
local CardStepComponent = ComponentType()
CardStepComponent.Name = "CardStep"
CardStepComponent:AddVariable("Step", ByteSize.Int)
CardStepComponent:AddVariable("UnitEntityId", ByteSize.Reference)
CardStepComponent.TableType = TableType.Map
worldCreator:AddComponentType(CardStepComponent)

-- Card Index Component
local CardIndexComponent = ComponentType()
CardIndexComponent.Name = "CardIndex"
CardIndexComponent:AddVariable("Index", ByteSize.Int)
CardIndexComponent.TableType = TableType.Map
worldCreator:AddComponentType(CardIndexComponent)

-- Select Card Component
local SelectCardComponent = ComponentType()
SelectCardComponent.Name = "SelectCard"
SelectCardComponent:AddVariable("Index", ByteSize.Int)
SelectCardComponent.TableType = TableType.Map
worldCreator:AddComponentType(SelectCardComponent)

-- Card Selected Component
local CardSelectedComponent = ComponentType()
CardSelectedComponent.Name = "CardSelected"
CardSelectedComponent.TableType = TableType.Map
worldCreator:AddComponentType(CardSelectedComponent)
