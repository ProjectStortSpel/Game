--	This file will contain
--	components that are needed
--	for unit components

--	Unit Component
local NewComponent = ComponentType()
NewComponent.Name = "Unit"
NewComponent.TableType = TableType.Map
NewComponent.SyncNetwork = true
worldCreator:AddComponentType(NewComponent)

--	MyUnit Component
NewComponent = ComponentType()
NewComponent.Name = "MyUnit"
NewComponent.TableType = TableType.Map
worldCreator:AddComponentType(NewComponent)

--	PlayerEntityId Component
NewComponent = ComponentType()
NewComponent.Name = "PlayerEntityId"
NewComponent.TableType = TableType.Map
NewComponent:AddVariable("Id", ByteSize.Reference)
worldCreator:AddComponentType(NewComponent)

-- Direction Component
NewComponent = ComponentType()
NewComponent.Name = "Direction"
NewComponent.SyncNetwork = true
NewComponent.TableType = TableType.Map
NewComponent:AddVariable("X", ByteSize.Int)
NewComponent:AddVariable("Z", ByteSize.Int)
worldCreator:AddComponentType(NewComponent)

-- Unit Dead Component
NewComponent = ComponentType()
NewComponent.Name = "UnitDead"
NewComponent.TableType = TableType.Map
worldCreator:AddComponentType(NewComponent)

-- NeedUnit Component
NewComponent = ComponentType()
NewComponent.Name = "NeedUnit"
NewComponent.TableType = TableType.Map
worldCreator:AddComponentType(NewComponent)

-- RemoveUnit Component
NewComponent = ComponentType()
NewComponent.Name = "RemoveUnit"
NewComponent.TableType = TableType.Map
NewComponent:AddVariable("PlayerNo", ByteSize.Int)
NewComponent:AddVariable("UnitEntityId", ByteSize.Int)
worldCreator:AddComponentType(NewComponent)

-- Spawn Component
NewComponent = ComponentType()
NewComponent.Name = "Spawn"
NewComponent.TableType = TableType.Map
NewComponent:AddVariable("UnitEntityId", ByteSize.Reference)
worldCreator:AddComponentType(NewComponent)

-- Spawnpoint Component
NewComponent = ComponentType()
NewComponent.Name = "Spawnpoint"
NewComponent.TableType = TableType.Map
NewComponent:AddVariable("X", ByteSize.Int)
NewComponent:AddVariable("Z", ByteSize.Int)
worldCreator:AddComponentType(NewComponent)

--	NeedSpawnLocation Component
NewComponent = ComponentType()
NewComponent.Name = "NeedSpawnLocation"
NewComponent.TableType = TableType.Map
worldCreator:AddComponentType(NewComponent)

-- TargetCheckpoint Component
NewComponent = ComponentType()
NewComponent.Name = "TargetCheckpoint"
NewComponent.TableType = TableType.Map
NewComponent:AddVariable("Id", ByteSize.Int)
worldCreator:AddComponentType(NewComponent)

-- TargetPosition Component
NewComponent = ComponentType()
NewComponent.Name = "TargetPosition"
NewComponent.TableType = TableType.Map
NewComponent:AddVariable("X", ByteSize.Float)
NewComponent:AddVariable("Y", ByteSize.Float)
NewComponent:AddVariable("Z", ByteSize.Float)
worldCreator:AddComponentType(NewComponent)

-- UnitSelectedCards Component
NewComponent = ComponentType()
NewComponent.Name = "UnitSelectedCards"
NewComponent.TableType = TableType.Map
worldCreator:AddComponentType(NewComponent)

NewComponent = ComponentType()
NewComponent.Name = "UnitWantTileOffset"
NewComponent.TableType = TableType.Map
worldCreator:AddComponentType(NewComponent)

-- UnitVisualizer Component
NewComponent = ComponentType()
NewComponent.Name = "UnitGhost"
NewComponent.TableType = TableType.Map
NewComponent:AddVariable("Id", ByteSize.Int)
worldCreator:AddComponentType(NewComponent)

-- UnitVisualizer Component
NewComponent = ComponentType()
NewComponent.Name = "CardAboveHead"
NewComponent.TableType = TableType.Map
NewComponent:AddVariable("CardOwner", ByteSize.Reference)
NewComponent:AddVariable("CardId", ByteSize.Reference)
NewComponent:AddVariable("CardDenied", ByteSize.Int)
worldCreator:AddComponentType(NewComponent)

NewComponent = ComponentType()
NewComponent.Name = "FallDownSound"
NewComponent.TableType = TableType.Map
NewComponent:AddVariable("X", ByteSize.Float)
NewComponent:AddVariable("Y", ByteSize.Float)
NewComponent:AddVariable("Z", ByteSize.Float)
worldCreator:AddComponentType(NewComponent)