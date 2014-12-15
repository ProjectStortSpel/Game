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

-- Turn Left Component
local TurnLeftComponent = ComponentType()
TurnLeftComponent.Name = "TurnLeft"
TurnLeftComponent.TableType = TableType.Array
worldCreator:AddComponentType(TurnLeftComponent)

-- Turn Right Component
local TurnRightComponent = ComponentType()
TurnRightComponent.Name = "TurnRight"
TurnRightComponent.TableType = TableType.Array
worldCreator:AddComponentType(TurnRightComponent)

-- Turn Around Component
local TurnAroundComponent = ComponentType()
TurnAroundComponent.Name = "TurnAround"
TurnAroundComponent.TableType = TableType.Array
worldCreator:AddComponentType(TurnAroundComponent)

-- Forward Component
local ForwardComponent = ComponentType()
ForwardComponent.Name = "Forward"
ForwardComponent.TableType = TableType.Array
worldCreator:AddComponentType(ForwardComponent)

-- Backward Component
local BackwardComponent = ComponentType()
BackwardComponent.Name = "Backward"
BackwardComponent.TableType = TableType.Array
worldCreator:AddComponentType(BackwardComponent)

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