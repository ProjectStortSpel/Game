-- Player Component
local PlayerComponent = ComponentType()
PlayerComponent.Name = "Player"
PlayerComponent.TableType = TableType.Array
worldCreator:AddComponentType(PlayerComponent)

-- Direction Component
local DirectionComponent = ComponentType()
DirectionComponent.Name = "Direction"
DirectionComponent.TableType = TableType.Array
DirectionComponent:AddVariable("X", ByteSize.Int)
DirectionComponent:AddVariable("Y", ByteSize.Int)
worldCreator:AddComponentType(DirectionComponent)

-- Spawn Component
local SpawnComponent = ComponentType()
SpawnComponent.Name = "Spawn"
SpawnComponent.TableType = TableType.Array
SpawnComponent:AddVariable("X", ByteSize.Int)
SpawnComponent:AddVariable("Y", ByteSize.Int)
worldCreator:AddComponentType(SpawnComponent)

-- TargetCheckpoint Component
local TargetCheckpointComponent = ComponentType()
TargetCheckpointComponent.Name = "TargetCheckpoint"
TargetCheckpointComponent.TableType = TableType.Array
TargetCheckpointComponent:AddVariable("Id", ByteSize.Int)
worldCreator:AddComponentType(TargetCheckpointComponent)

-- TargetPosition Component
local TargetPositionComponent = ComponentType()
TargetPositionComponent.Name = "TargetPosition"
TargetPositionComponent.TableType = TableType.Array
TargetPositionComponent:AddVariable("X", ByteSize.Float)
TargetPositionComponent:AddVariable("Y", ByteSize.Float)
TargetPositionComponent:AddVariable("Z", ByteSize.Float)
worldCreator:AddComponentType(TargetPositionComponent)

-- Inactive Player Component
local InactivePlayerComponent = ComponentType()
InactivePlayerComponent.Name = "InactivePlayer"
InactivePlayerComponent.TableType = TableType.Map
worldCreator:AddComponentType(InactivePlayerComponent)