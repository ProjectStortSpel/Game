--	This file will contain
--	components that are needed
--	for the cards

-- Dealing Settings Component
local NewComponent = ComponentType()
NewComponent.SyncNetwork = true
NewComponent.Name = "DealingSettings"
NewComponent.TableType = TableType.Map
NewComponent:AddVariable("CardsInHand", ByteSize.Int)
NewComponent:AddVariable("CardsToPick", ByteSize.Int)
worldCreator:AddComponentType(NewComponent)

-- Card Component
NewComponent = ComponentType()
NewComponent.SyncNetwork = true
NewComponent.Name = "Card"
NewComponent.TableType = TableType.Map
NewComponent:AddVariable("Unit", ByteSize.Reference)
worldCreator:AddComponentType(NewComponent)

-- Card Prio Component
NewComponent = ComponentType()
NewComponent.Name = "CardPrio"
NewComponent.SyncNetwork = true
NewComponent.TableType = TableType.Map
NewComponent:AddVariable("Prio", ByteSize.Int)
worldCreator:AddComponentType(NewComponent)

-- Card Action Component
NewComponent = ComponentType()
NewComponent.Name = "CardAction"
NewComponent.SyncNetwork = true
NewComponent.TableType = TableType.Map
NewComponent:AddVariable("Action", ByteSize.Text)
worldCreator:AddComponentType(NewComponent)

-- Defines that the card is owned by an AI.
-- AI Card Component
NewComponent = ComponentType()
NewComponent.Name = "AICard"
NewComponent.TableType = TableType.Map
worldCreator:AddComponentType(NewComponent)

-- Used Card Component
NewComponent = ComponentType()
NewComponent.Name = "UsedCard"
NewComponent.TableType = TableType.Map
worldCreator:AddComponentType(NewComponent)

-- Dealt Card Component
NewComponent = ComponentType()
NewComponent.Name = "DealtCard"
NewComponent:AddVariable("PlayerEntityId", ByteSize.Reference)
NewComponent.TableType = TableType.Map
worldCreator:AddComponentType(NewComponent)

-- Card Step Component
NewComponent = ComponentType()
NewComponent.Name = "CardStep"
NewComponent:AddVariable("Step", ByteSize.Int)
NewComponent:AddVariable("UnitEntityId", ByteSize.Reference)
NewComponent.TableType = TableType.Map
worldCreator:AddComponentType(NewComponent)

-- Card Index Component
NewComponent = ComponentType()
NewComponent.Name = "CardIndex"
NewComponent:AddVariable("Index", ByteSize.Int)
NewComponent.TableType = TableType.Map
worldCreator:AddComponentType(NewComponent)

-- Select Card Component
NewComponent = ComponentType()
NewComponent.Name = "SelectCard"
NewComponent:AddVariable("Index", ByteSize.Int)
NewComponent.TableType = TableType.Map
worldCreator:AddComponentType(NewComponent)

-- Card Selected Component
NewComponent = ComponentType()
NewComponent.Name = "CardSelected"
NewComponent.TableType = TableType.Map
worldCreator:AddComponentType(NewComponent)

-- Card Holding Component
NewComponent = ComponentType()
NewComponent.Name = "CardHolding"
NewComponent.TableType = TableType.Map
worldCreator:AddComponentType(NewComponent)


-- Play Card Timer
NewComponent = ComponentType()
NewComponent.Name = "PlayCardTimer"
NewComponent:AddVariable("Time", ByteSize.Float)
NewComponent:AddVariable("Step", ByteSize.Int)
NewComponent.TableType = TableType.Map
worldCreator:AddComponentType(NewComponent)

--	Step Timer
NewComponent = ComponentType()
NewComponent.Name = "StepTimer"
NewComponent:AddVariable("Time", ByteSize.Float)
NewComponent.TableType = TableType.Map
worldCreator:AddComponentType(NewComponent)

-- Server Selected Card component
NewComponent = ComponentType()
NewComponent.Name = "ServerSelectedCard"
NewComponent.TableType = TableType.Map
NewComponent:AddVariable("Index", ByteSize.Int)
worldCreator:AddComponentType(NewComponent)

--	Update Spawn
NewComponent = ComponentType()
NewComponent.Name = "UpdateSpawnpoint"
NewComponent:AddVariable("Unit", ByteSize.Reference)
NewComponent.TableType = TableType.Map
worldCreator:AddComponentType(NewComponent)