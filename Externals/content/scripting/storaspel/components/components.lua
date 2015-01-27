-- Position Component
local PositionComponent = ComponentType()
PositionComponent.Name = "Position"
PositionComponent.SyncNetwork = true
PositionComponent.TableType = TableType.Array
PositionComponent:AddVariable("X", ByteSize.Float)
PositionComponent:AddVariable("Y", ByteSize.Float)
PositionComponent:AddVariable("Z", ByteSize.Float)
worldCreator:AddComponentType(PositionComponent)

-- Scale Component
local ScaleComponent = ComponentType()
ScaleComponent.Name = "Scale"
ScaleComponent.SyncNetwork = true
ScaleComponent.TableType = TableType.Array
ScaleComponent:AddVariable("X", ByteSize.Float)
ScaleComponent:AddVariable("Y", ByteSize.Float)
ScaleComponent:AddVariable("Z", ByteSize.Float)
worldCreator:AddComponentType(ScaleComponent)

-- Rotation Component
local RotationComponent = ComponentType()
RotationComponent.Name = "Rotation"
RotationComponent.SyncNetwork = true
RotationComponent.TableType = TableType.Array
RotationComponent:AddVariable("X", ByteSize.Float)
RotationComponent:AddVariable("Y", ByteSize.Float)
RotationComponent:AddVariable("Z", ByteSize.Float)
worldCreator:AddComponentType(RotationComponent)


-- Slerp Rotation Component
local SlerpRotationComponent = ComponentType()
SlerpRotationComponent.Name = "SlerpRotation"
SlerpRotationComponent.SyncNetwork = true
SlerpRotationComponent.TableType = TableType.Map
SlerpRotationComponent:AddVariable("fromX", ByteSize.Float)
SlerpRotationComponent:AddVariable("fromY", ByteSize.Float)
SlerpRotationComponent:AddVariable("fromZ", ByteSize.Float)
SlerpRotationComponent:AddVariable("fromW", ByteSize.Float)
SlerpRotationComponent:AddVariable("time", ByteSize.Float)
SlerpRotationComponent:AddVariable("toX", ByteSize.Float)
SlerpRotationComponent:AddVariable("toY", ByteSize.Float)
SlerpRotationComponent:AddVariable("toZ", ByteSize.Float)
SlerpRotationComponent:AddVariable("toW", ByteSize.Float)
worldCreator:AddComponentType(SlerpRotationComponent)

-- Velocity Component
local VelocityComponent = ComponentType()
VelocityComponent.Name = "Velocity"
VelocityComponent.SyncNetwork = true
VelocityComponent.TableType = TableType.Array
VelocityComponent:AddVariable("X", ByteSize.Float)
VelocityComponent:AddVariable("Y", ByteSize.Float)
VelocityComponent:AddVariable("Z", ByteSize.Float)
worldCreator:AddComponentType(VelocityComponent)

-- Render Component
local RenderComponent = ComponentType()
RenderComponent.Name = "Render"
RenderComponent.TableType = TableType.Array
RenderComponent:AddVariable("ModelId", ByteSize.Int)
RenderComponent:AddVariable("Mat", ByteSize.Matrix)
worldCreator:AddComponentType(RenderComponent)

-- Hide Component
local HideComponent = ComponentType()
HideComponent.Name = "Hide"
HideComponent.SyncNetwork = true
HideComponent.TableType = TableType.Map
worldCreator:AddComponentType(HideComponent)


--	Model Component
local ModelComponent = ComponentType()
ModelComponent.Name = "Model"
ModelComponent.SyncNetwork = true
ModelComponent.TableType = TableType.Map
ModelComponent:AddVariable("ModelName", ByteSize.Text)
ModelComponent:AddVariable("ModelPath", ByteSize.Text)
ModelComponent:AddVariable("RenderType", ByteSize.Int)
worldCreator:AddComponentType(ModelComponent)

-- Spin Component
local SpinComponent = ComponentType()
SpinComponent.Name = "Spin"
SpinComponent.SyncNetwork = true
SpinComponent.TableType = TableType.Array
SpinComponent:AddVariable("X", ByteSize.Float)
SpinComponent:AddVariable("Y", ByteSize.Float)
SpinComponent:AddVariable("Z", ByteSize.Float)
worldCreator:AddComponentType(SpinComponent)

-- Camera Component
local CameraComponent = ComponentType()
CameraComponent.Name = "Camera"
CameraComponent.SyncNetwork = true
CameraComponent.TableType = TableType.Map
CameraComponent:AddVariable("EntityId", ByteSize.Reference)
worldCreator:AddComponentType(CameraComponent)

-- Network Component
local NetworkComponent = ComponentType()
NetworkComponent.Name = "Network"
NetworkComponent.SyncNetwork = true
NetworkComponent.TableType = TableType.Map
worldCreator:AddComponentType(NetworkComponent)

-- Username Component
local NameComponent = ComponentType()
NameComponent.Name = "Username"
NameComponent.SyncNetwork = true
NameComponent.TableType = TableType.Map
NameComponent:AddVariable("Name", ByteSize.Text)
worldCreator:AddComponentType(NameComponent)

-- NetConnection Component
local NetConnection = ComponentType()
NetConnection.Name = "NetConnection"
NetConnection.TableType = TableType.Map
NetConnection:AddVariable("IpAddress", ByteSize.Text)
NetConnection:AddVariable("Port", ByteSize.Int)
worldCreator:AddComponentType(NetConnection)

-- Active NetConnection Component
local ActiveNetConnection = ComponentType()
ActiveNetConnection.Name = "ActiveNetConnection"
ActiveNetConnection.TableType = TableType.Map
worldCreator:AddComponentType(ActiveNetConnection)

-- Sync Network Component
local SyncNetworkComponent = ComponentType()
SyncNetworkComponent.Name = "SyncNetwork"
SyncNetworkComponent.SyncNetwork = true
SyncNetworkComponent.TableType = TableType.Array
worldCreator:AddComponentType(SyncNetworkComponent)

-- Null Component
local NullComponent = ComponentType()
NullComponent.Name = "Null"
NullComponent.SyncNetwork = true
NullComponent.TableType = TableType.Map
worldCreator:AddComponentType(NullComponent)


-- TotemPole Component
local TotemPoleComponent = ComponentType()
TotemPoleComponent.Name = "TotemPole"
TotemPoleComponent:AddVariable("Height", ByteSize.Int)
TotemPoleComponent.TableType = TableType.Map
worldCreator:AddComponentType(TotemPoleComponent)

-- CheckpointId Component
local CheckpointIdComponent = ComponentType()
CheckpointIdComponent.Name = "CheckpointId"
CheckpointIdComponent.TableType = TableType.Map
CheckpointIdComponent:AddVariable("Id", ByteSize.Int)
worldCreator:AddComponentType(CheckpointIdComponent)

-- TILE Component
local TileComponent = ComponentType()
TileComponent.Name = "TileComp"
TileComponent.TableType = TableType.Map
worldCreator:AddComponentType(TileComponent)