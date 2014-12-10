
-- Position Component
local PositionComponent = ComponentType()
PositionComponent.Name = "Position"
PositionComponent.TableType = TableType.Array
PositionComponent:AddVariable("X", 4)
PositionComponent:AddVariable("Y", 4)
PositionComponent:AddVariable("Z", ByteSize.Float)
worldCreator:AddComponentType(PositionComponent)

-- Scale Component
local ScaleComponent = ComponentType()
ScaleComponent.Name = "Scale"
ScaleComponent.TableType = TableType.Array
ScaleComponent:AddVariable("X", 4)
ScaleComponent:AddVariable("Y", 4)
ScaleComponent:AddVariable("Z", ByteSize.Float)
worldCreator:AddComponentType(ScaleComponent)

-- Rotation Component
local RotationComponent = ComponentType()
RotationComponent.Name = "Rotation"
RotationComponent.TableType = TableType.Array
RotationComponent:AddVariable("X", 4)
RotationComponent:AddVariable("Y", 4)
RotationComponent:AddVariable("Z", ByteSize.Float)
worldCreator:AddComponentType(RotationComponent)

-- Velocity Component
local VelocityComponent = ComponentType()
VelocityComponent.Name = "Velocity"
VelocityComponent.TableType = TableType.Array
VelocityComponent:AddVariable("X", 4)
VelocityComponent:AddVariable("Y", 4)
VelocityComponent:AddVariable("Z", ByteSize.Float)
worldCreator:AddComponentType(VelocityComponent)

-- Render Component
local RenderComponent = ComponentType()
RenderComponent.Name = "Render"
RenderComponent.TableType = TableType.Array
RenderComponent:AddVariable("ModelId", 4)
RenderComponent:AddVariable("Mat", 64)
worldCreator:AddComponentType(RenderComponent)

-- Spin Component
local SpinComponent = ComponentType()
SpinComponent.Name = "Spin"
SpinComponent.TableType = TableType.Array
SpinComponent:AddVariable("X", 4)
SpinComponent:AddVariable("Y", 4)
SpinComponent:AddVariable("Z", 4)
worldCreator:AddComponentType(SpinComponent)

-- Camera Component
local CameraComponent = ComponentType()
CameraComponent.Name = "Camera"
CameraComponent.TableType = TableType.Map
CameraComponent:AddVariable("EntityId", 4)
worldCreator:AddComponentType(CameraComponent)

-- Network Component
local NetworkComponent = ComponentType()
NetworkComponent.Name = "Network"
NetworkComponent.TableType = TableType.Map
worldCreator:AddComponentType(NetworkComponent)

