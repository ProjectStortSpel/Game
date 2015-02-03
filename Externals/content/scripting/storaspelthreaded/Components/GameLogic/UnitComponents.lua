--	This file will contain
--	components that are needed
--	for unit components

--	Unit Component
local UnitComponent = ComponentType()
UnitComponent.Name = "Unit"
UnitComponent.TableType = TableType.Map
UnitComponent.SyncNetwork = true
worldCreator:AddComponentType(UnitComponent)

--	PlayerEntityId Component
local PlayerEntityIdComponent = ComponentType()
PlayerEntityIdComponent.Name = "PlayerEntityId"
PlayerEntityIdComponent.TableType = TableType.Map
PlayerEntityIdComponent:AddVariable("Id", ByteSize.Reference)
worldCreator:AddComponentType(PlayerEntityIdComponent)

-- Direction Component
local DirectionComponent = ComponentType()
DirectionComponent.Name = "Direction"
DirectionComponent.TableType = TableType.Map
DirectionComponent:AddVariable("X", ByteSize.Int)
DirectionComponent:AddVariable("Z", ByteSize.Int)
worldCreator:AddComponentType(DirectionComponent)

-- Unit Dead Component
local UnitDeadComponent = ComponentType()
UnitDeadComponent.Name = "UnitDead"
UnitDeadComponent.TableType = TableType.Map
worldCreator:AddComponentType(UnitDeadComponent)

-- NeedUnit Component
local NeedUnitComponent = ComponentType()
NeedUnitComponent.Name = "NeedUnit"
NeedUnitComponent.TableType = TableType.Map
worldCreator:AddComponentType(NeedUnitComponent)

-- RemoveUnit Component
local RemoveUnitComponent = ComponentType()
RemoveUnitComponent.Name = "RemoveUnit"
RemoveUnitComponent.TableType = TableType.Map
RemoveUnitComponent:AddVariable("PlayerNo", ByteSize.Int)
RemoveUnitComponent:AddVariable("UnitEntityId", ByteSize.Int)
worldCreator:AddComponentType(RemoveUnitComponent)

-- Spawn Component
local SpawnComponent = ComponentType()
SpawnComponent.Name = "Spawn"
SpawnComponent.TableType = TableType.Array
worldCreator:AddComponentType(SpawnComponent)

-- Spawnpoint Component
local SpawnComponent = ComponentType()
SpawnComponent.Name = "Spawnpoint"
SpawnComponent.TableType = TableType.Map
SpawnComponent:AddVariable("X", ByteSize.Int)
SpawnComponent:AddVariable("Z", ByteSize.Int)
worldCreator:AddComponentType(SpawnComponent)

--	NeedSpawnLocation Component
local NeedSpawnLocation = ComponentType()
NeedSpawnLocation.Name = "NeedSpawnLocation"
NeedSpawnLocation.TableType = TableType.Map
worldCreator:AddComponentType(NeedSpawnLocation)

-- TargetCheckpoint Component
local TargetCheckpointComponent = ComponentType()
TargetCheckpointComponent.Name = "TargetCheckpoint"
TargetCheckpointComponent.TableType = TableType.Map
TargetCheckpointComponent:AddVariable("Id", ByteSize.Int)
worldCreator:AddComponentType(TargetCheckpointComponent)

-- TargetPosition Component
local TargetPositionComponent = ComponentType()
TargetPositionComponent.Name = "TargetPosition"
TargetPositionComponent.TableType = TableType.Map
TargetPositionComponent:AddVariable("X", ByteSize.Float)
TargetPositionComponent:AddVariable("Y", ByteSize.Float)
TargetPositionComponent:AddVariable("Z", ByteSize.Float)
worldCreator:AddComponentType(TargetPositionComponent)

-- UnitSelectedCards Component
local UnitSelectedCardsComponent = ComponentType()
UnitSelectedCardsComponent.Name = "UnitSelectedCards"
UnitSelectedCardsComponent.TableType = TableType.Map
worldCreator:AddComponentType(UnitSelectedCardsComponent)

