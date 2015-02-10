--	This file will contain
--	components that are needed
--	for the cards

-- Card Component
local CardComponent = ComponentType()
CardComponent.SyncNetwork = true
CardComponent.Name = "Card"
CardComponent.TableType = TableType.Map
worldCreator:AddComponentType(CardComponent)

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

-- Defines that the card is owned by an AI.
-- AI Card Component
local AICardComponent = ComponentType()
AICardComponent.Name = "AICard"
AICardComponent.TableType = TableType.Map
worldCreator:AddComponentType(AICardComponent)

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

-- Card Holding Component
local CardHoldingComponent = ComponentType()
CardHoldingComponent.Name = "CardHolding"
CardHoldingComponent.TableType = TableType.Map
worldCreator:AddComponentType(CardHoldingComponent)


-- Play Card Timer
local PlayCardTimerComponent = ComponentType()
PlayCardTimerComponent.Name = "PlayCardTimer"
PlayCardTimerComponent:AddVariable("Time", ByteSize.Float)
PlayCardTimerComponent:AddVariable("Step", ByteSize.Int)
PlayCardTimerComponent.TableType = TableType.Map
worldCreator:AddComponentType(PlayCardTimerComponent)

--	Step Timer
local StepTimerComponent = ComponentType()
StepTimerComponent.Name = "StepTimer"
StepTimerComponent:AddVariable("Time", ByteSize.Float)
StepTimerComponent.TableType = TableType.Map
worldCreator:AddComponentType(StepTimerComponent)

--	Update Spawn
local UpdateSpawnpointComponent = ComponentType()
UpdateSpawnpointComponent.Name = "UpdateSpawnpoint"
UpdateSpawnpointComponent:AddVariable("Unit", ByteSize.Reference)
UpdateSpawnpointComponent.TableType = TableType.Map
worldCreator:AddComponentType(UpdateSpawnpointComponent)