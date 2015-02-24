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
DirectionComponent.SyncNetwork = true
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
SpawnComponent.TableType = TableType.Map
SpawnComponent:AddVariable("UnitEntityId", ByteSize.Reference)
worldCreator:AddComponentType(SpawnComponent)

-- Spawnpoint Component
local SpawnpointComponent = ComponentType()
SpawnpointComponent.Name = "Spawnpoint"
SpawnpointComponent.TableType = TableType.Map
SpawnpointComponent:AddVariable("X", ByteSize.Int)
SpawnpointComponent:AddVariable("Z", ByteSize.Int)
worldCreator:AddComponentType(SpawnpointComponent)

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

local UnitWantTileOffset = ComponentType()
UnitWantTileOffset.Name = "UnitWantTileOffset"
UnitWantTileOffset.TableType = TableType.Map
worldCreator:AddComponentType(UnitWantTileOffset)

-- UnitVisualizer Component
local UnitGhost = ComponentType()
UnitGhost.Name = "UnitGhost"
UnitGhost.TableType = TableType.Map
UnitGhost:AddVariable("Id", ByteSize.Reference)
worldCreator:AddComponentType(UnitGhost)

