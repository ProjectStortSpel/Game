
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
RenderComponent.SyncNetwork = false
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

-- Username Component
local NameComponent = ComponentType()
NameComponent.Name = "Username"
NameComponent.TableType = TableType.Map
NameComponent:AddVariable("Name", ByteSize.Text)
worldCreator:AddComponentType(NameComponent)

-- NetConnection Component
local NetConnection = ComponentType()
NetConnection.Name = "NetConnection"
NetConnection.TableType = TableType.Map
NetConnection:AddVariable("IpAddress", ByteSize.Text)
NetConnection:AddVariable("Port", ByteSize.Int)
NetConnection:AddVariable("Active", ByteSize.Bool)
worldCreator:AddComponentType(NetConnection)