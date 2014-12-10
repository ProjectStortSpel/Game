
-- Position Component
local PositionComponent = ComponentType()
PositionComponent.Name = "Position"
PositionComponent.TableType = TableType.Array
PositionComponent:AddVariable("X", ByteSize.Float)
PositionComponent:AddVariable("Y", ByteSize.Float)
PositionComponent:AddVariable("Z", ByteSize.Float)
worldCreator:AddComponentType(PositionComponent)

-- Scale Component
local ScaleComponent = ComponentType()
ScaleComponent.Name = "Scale"
ScaleComponent.TableType = TableType.Array
ScaleComponent:AddVariable("X", ByteSize.Float)
ScaleComponent:AddVariable("Y", ByteSize.Float)
ScaleComponent:AddVariable("Z", ByteSize.Float)
worldCreator:AddComponentType(ScaleComponent)

-- Rotation Component
local RotationComponent = ComponentType()
RotationComponent.Name = "Rotation"
RotationComponent.TableType = TableType.Array
RotationComponent:AddVariable("X", ByteSize.Float)
RotationComponent:AddVariable("Y", ByteSize.Float)
RotationComponent:AddVariable("Z", ByteSize.Float)
worldCreator:AddComponentType(RotationComponent)

-- Velocity Component
local VelocityComponent = ComponentType()
VelocityComponent.Name = "Velocity"
VelocityComponent.TableType = TableType.Array
VelocityComponent:AddVariable("X", ByteSize.Float)
VelocityComponent:AddVariable("Y", ByteSize.Float)
VelocityComponent:AddVariable("Z", ByteSize.Float)
worldCreator:AddComponentType(VelocityComponent)

-- Render Component
local RenderComponent = ComponentType()
RenderComponent.Name = "Render"
PositionComponent.SyncNetwork = true
RenderComponent.TableType = TableType.Array
RenderComponent:AddVariable("ModelId", ByteSize.Int)
RenderComponent:AddVariable("Mat", ByteSize.Matrix)
worldCreator:AddComponentType(RenderComponent)

--	Model Component
local ModelComponent = ComponentType()
ModelComponent.Name = "Model"
ModelComponent.TableType = TableType.Map
ModelComponent:AddVariable("ModelName", ByteSize.Text)
ModelComponent:AddVariable("ModelPath", ByteSize.Text)
worldCreator:AddComponentType(ModelComponent)

-- Spin Component
local SpinComponent = ComponentType()
SpinComponent.Name = "Spin"
SpinComponent.TableType = TableType.Array
SpinComponent:AddVariable("X", ByteSize.Float)
SpinComponent:AddVariable("Y", ByteSize.Float)
SpinComponent:AddVariable("Z", ByteSize.Float)
worldCreator:AddComponentType(SpinComponent)

-- Camera Component
local CameraComponent = ComponentType()
CameraComponent.Name = "Camera"
CameraComponent.TableType = TableType.Map
CameraComponent:AddVariable("EntityId", ByteSize.Reference)
worldCreator:AddComponentType(CameraComponent)

-- Network Component
local NetworkComponent = ComponentType()
NetworkComponent.Name = "Network"
NetworkComponent.TableType = TableType.Map
worldCreator:AddComponentType(NetworkComponent)

-- Direction Component
local DirectionComponent = ComponentType()
DirectionComponent.Name = "Direction"
DirectionComponent.TableType = TableType.Array
DirectionComponent:AddVariable("X", ByteSize.Int)
DirectionComponent:AddVariable("Y", ByteSize.Int)
worldCreator:AddComponentType(DirectionComponent)

-- Move Forward Component
local MoveForwardComponent = ComponentType()
MoveForwardComponent.Name = "Forward"
MoveForwardComponent.TableType = TableType.Array
worldCreator:AddComponentType(MoveForwardComponent)

-- Move Backward Component
local MoveBackwardComponent = ComponentType()
MoveBackwardComponent.Name = "Backward"
MoveBackwardComponent.TableType = TableType.Array
worldCreator:AddComponentType(MoveBackwardComponent)

-- Turn Right Component
local TurnRightComponent = ComponentType()
TurnRightComponent.Name = "TurnRight"
TurnRightComponent.TableType = TableType.Array
worldCreator:AddComponentType(TurnRightComponent)

-- Turn Left Component
local TurnLeftComponent = ComponentType()
TurnLeftComponent.Name = "TurnLeft"
TurnLeftComponent.TableType = TableType.Array
worldCreator:AddComponentType(TurnLeftComponent)

-- Target Position Component
local TargetPositionComponent = ComponentType()
TargetPositionComponent.Name = "TargetPosition"
TargetPositionComponent.TableType = TableType.Array
SpinComponent:AddVariable("X", ByteSize.Float)
SpinComponent:AddVariable("Y", ByteSize.Float)
SpinComponent:AddVariable("Z", ByteSize.Float)
worldCreator:AddComponentType(TargetPositionComponent)

