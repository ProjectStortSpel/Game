-- Turn Left Component
local CardComponent = ComponentType()
CardComponent.SyncNetwork = true
CardComponent.Name = "Card"
CardComponent.TableType = TableType.Map
worldCreator:AddComponentType(CardComponent)

-- Turn Left Component
local TurnLeftComponent = ComponentType()
TurnLeftComponent.SyncNetwork = false
TurnLeftComponent.Name = "TurnLeft"
TurnLeftComponent.TableType = TableType.Map
worldCreator:AddComponentType(TurnLeftComponent)

-- Turn Right Component
local TurnRightComponent = ComponentType()
TurnRightComponent.SyncNetwork = false
TurnRightComponent.Name = "TurnRight"
TurnRightComponent.TableType = TableType.Map
worldCreator:AddComponentType(TurnRightComponent)

-- Turn Around Component
local TurnAroundComponent = ComponentType()
TurnAroundComponent.SyncNetwork = false
TurnAroundComponent.Name = "TurnAround"
TurnAroundComponent.TableType = TableType.Map
worldCreator:AddComponentType(TurnAroundComponent)

-- Forward Component
local ForwardComponent = ComponentType()
ForwardComponent.SyncNetwork = false
ForwardComponent.Name = "Forward"
ForwardComponent.TableType = TableType.Map
worldCreator:AddComponentType(ForwardComponent)

-- Backward Component
local BackwardComponent = ComponentType()
BackwardComponent.SyncNetwork = false
BackwardComponent.Name = "Backward"
BackwardComponent.TableType = TableType.Map
worldCreator:AddComponentType(BackwardComponent)

-- Card Prio Component
local CardPrioComponent = ComponentType()
CardPrioComponent.Name = "CardPrio"
CardPrioComponent.TableType = TableType.Map
CardPrioComponent:AddVariable("Prio", ByteSize.Int)
worldCreator:AddComponentType(CardPrioComponent)

-- Card Action Component
local CardActionComponent = ComponentType()
CardActionComponent.Name = "CardAction"
CardActionComponent.TableType = TableType.Map
CardActionComponent:AddVariable("Action", ByteSize.Text)
worldCreator:AddComponentType(CardActionComponent)

-- Used Card Component
local UsedCardComponent = ComponentType()
UsedCardComponent.SyncNetwork = false
UsedCardComponent.Name = "UsedCard"
UsedCardComponent.TableType = TableType.Map
worldCreator:AddComponentType(UsedCardComponent)

-- Dealt Card Component
local DealtCardComponent = ComponentType()
DealtCardComponent.SyncNetwork = false
DealtCardComponent.Name = "DealtCard"
DealtCardComponent:AddVariable("PlayerEntityId", ByteSize.Reference)
DealtCardComponent.TableType = TableType.Map
worldCreator:AddComponentType(DealtCardComponent)

-- Card Step Component
local CardStepComponent = ComponentType()
CardStepComponent.SyncNetwork = false
CardStepComponent.Name = "CardStep"
CardStepComponent:AddVariable("Step", ByteSize.Int)
CardStepComponent:AddVariable("UnitEntityId", ByteSize.Reference)
CardStepComponent.TableType = TableType.Map
worldCreator:AddComponentType(CardStepComponent)

-- Card Index Component
local CardIndexComponent = ComponentType()
CardIndexComponent.SyncNetwork = false
CardIndexComponent.Name = "CardIndex"
CardIndexComponent:AddVariable("Index", ByteSize.Int)
CardIndexComponent.TableType = TableType.Map
worldCreator:AddComponentType(CardIndexComponent)

-- Select Card Component
local SelectCardComponent = ComponentType()
SelectCardComponent.SyncNetwork = false
SelectCardComponent.Name = "SelectCard"
SelectCardComponent:AddVariable("Index", ByteSize.Int)
SelectCardComponent.TableType = TableType.Map
worldCreator:AddComponentType(SelectCardComponent)

-- Card Selected Component
local CardSelectedComponent = ComponentType()
CardSelectedComponent.SyncNetwork = false
CardSelectedComponent.Name = "CardSelected"
CardSelectedComponent.TableType = TableType.Map
worldCreator:AddComponentType(CardSelectedComponent)

