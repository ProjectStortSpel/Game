
--	Player Component
local PlayerComponent = ComponentType()
PlayerComponent.Name = "Player"
PlayerComponent.TableType = TableType.Map
worldCreator:AddComponentType(PlayerComponent)

--	Unit Component
local UnitComponent = ComponentType()
UnitComponent.Name = "Unit"
UnitComponent.TableType = TableType.Map
worldCreator:AddComponentType(UnitComponent)

--	PlayerNumber Component
local PlayerNumberComponent = ComponentType()
PlayerNumberComponent.Name = "PlayerNumber"
PlayerNumberComponent.TableType = TableType.Map
PlayerNumberComponent:AddVariable("Number", ByteSize.Int)
worldCreator:AddComponentType(PlayerNumberComponent)

--	PlayerName Component
local PlayerNameComponent = ComponentType()
PlayerNameComponent.Name = "PlayerName"
PlayerNameComponent.TableType = TableType.Map
PlayerNameComponent:AddVariable("Name", ByteSize.Text)
worldCreator:AddComponentType(PlayerNameComponent)

--	PlayerEntityId Component
local PlayerEntityIdComponent = ComponentType()
PlayerEntityIdComponent.Name = "PlayerEntityId"
PlayerEntityIdComponent.TableType = TableType.Map
PlayerEntityIdComponent:AddVariable("Id", ByteSize.Reference)
worldCreator:AddComponentType(PlayerEntityIdComponent)

--	UnitEntityId Component
local UnitEntityIdComponent = ComponentType()
UnitEntityIdComponent.Name = "UnitEntityId"
UnitEntityIdComponent.TableType = TableType.Map
UnitEntityIdComponent:AddVariable("Id", ByteSize.Reference)
worldCreator:AddComponentType(UnitEntityIdComponent)

-- Direction Component
local DirectionComponent = ComponentType()
DirectionComponent.Name = "Direction"
DirectionComponent.TableType = TableType.Map
DirectionComponent:AddVariable("X", ByteSize.Int)
DirectionComponent:AddVariable("Z", ByteSize.Int)
worldCreator:AddComponentType(DirectionComponent)

-- Spawn Component
local SpawnComponent = ComponentType()
SpawnComponent.Name = "Spawn"
SpawnComponent.TableType = TableType.Array
worldCreator:AddComponentType(SpawnComponent)

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

-- Inactive Player Component
local InactivePlayerComponent = ComponentType()
InactivePlayerComponent.Name = "InactivePlayer"
InactivePlayerComponent.TableType = TableType.Map
worldCreator:AddComponentType(InactivePlayerComponent)

-- Unit Dead Component
local UnitDeadComponent = ComponentType()
UnitDeadComponent.Name = "UnitDead"
UnitDeadComponent.TableType = TableType.Map
worldCreator:AddComponentType(UnitDeadComponent)

-- Spawnpoint Component
local SpawnComponent = ComponentType()
SpawnComponent.Name = "Spawnpoint"
SpawnComponent.TableType = TableType.Map
SpawnComponent:AddVariable("X", ByteSize.Int)
SpawnComponent:AddVariable("Z", ByteSize.Int)
worldCreator:AddComponentType(SpawnComponent)

-- UnitSelectedCards Component
local UnitSelectedCardsComponent = ComponentType()
UnitSelectedCardsComponent.Name = "UnitSelectedCards"
UnitSelectedCardsComponent.TableType = TableType.Map
worldCreator:AddComponentType(UnitSelectedCardsComponent)

--	PickingDone Component
local PickingDone = ComponentType()
PickingDone.Name = "PickingDone"
PickingDone.TableType = TableType.Map
PickingDone:AddVariable("Done", ByteSize.Int)
worldCreator:AddComponentType(PickingDone)

--	NeedSpawnLocation Component
local NeedSpawnLocation = ComponentType()
NeedSpawnLocation.Name = "NeedSpawnLocation"
NeedSpawnLocation.TableType = TableType.Map
worldCreator:AddComponentType(NeedSpawnLocation)


-- Available Spawnpoint
local AvailableSpawnpoint = ComponentType()
AvailableSpawnpoint.Name = "AvailableSpawnpoint"
AvailableSpawnpoint.TableType = TableType.Map
AvailableSpawnpoint:AddVariable("X", ByteSize.Int)
AvailableSpawnpoint:AddVariable("Z", ByteSize.Int)
worldCreator:AddComponentType(AvailableSpawnpoint)










